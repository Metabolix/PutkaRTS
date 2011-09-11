/*
 * GUI game handler class implementation.
 *
 * Copyright 2011 Lauri Kenttä
 * Copyright 2011 Mika Katajamäki
 *
 * This file is part of PutkaRTS.
 *
 * PutkaRTS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PutkaRTS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PutkaRTS.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <algorithm>
#include <cmath>
#include <boost/bind.hpp>

#include "util/Path.hpp"

#include "game/Game.hpp"
#include "game/Map.hpp"
#include "connection/Client.hpp"

#include "gui/GUI.hpp"

#include "gui/game/Game.hpp"
#include "gui/game/Object.hpp"

#include "gui/widget/Button.hpp"

#include <SFML/Graphics.hpp>

GUI::Game::Game::Game(boost::shared_ptr<Connection::Client> connection_, sf::RenderWindow& window):
	guiView(window.GetDefaultView()),
	connection(connection_),
	gameView(window, sf::Vector2f(connection->getGame().getMap().getSizeX(), connection->getGame().getMap().getSizeY()), 32) {

	loadMapData();

	// TODO: Center at the player start position or something.
	gameView.SetCenter(
		connection->getGame().getMap().getSizeX() / 2,
		connection->getGame().getMap().getSizeY() / 2
	);

	insert(new GUI::Widget::Button("X", window.GetWidth() - 24, 0, 24, 24, boost::bind(&Game::exit, this)));
	insert(new GUI::Widget::Button("S", window.GetWidth() - 48, 0, 24, 24, boost::bind(&Game::openSettingsMenu, this, boost::ref(window))));

	connection->setReadyToStart();
}

void GUI::Game::Game::loadMapData() {
	const ::Game::Map& map = connection->getGame().getMap();
	const ::Game::Map::TileInfoMap& tileInfoMap = map.getTileInfoMap();

	for (::Game::Map::TileInfoMap::const_iterator i = tileInfoMap.begin(); i != tileInfoMap.end(); ++i) {
		const ::Game::Map::TileInfo& info = i->second;
		images.get(info.texture, Path::findDataPath(map.getDirectory(), "", info.texture));
	}
}

void GUI::Game::Game::drawGame(sf::RenderWindow& window) const {
	const ::Game::Game& game = connection->getGame();
	const ::Game::Map& map = game.getMap();

	//calculate which tiles are on the screen.
	::Game::Map::SizeType beginY = std::max(0.0f, gameView.GetRect().Top);
	::Game::Map::SizeType endY = std::min< ::Game::Map::SizeType>(map.getSizeY(), std::max(0.0f, std::ceil(gameView.GetRect().Bottom)));
	::Game::Map::SizeType beginX = std::max(0.0f, gameView.GetRect().Left);
	::Game::Map::SizeType endX = std::min< ::Game::Map::SizeType>(map.getSizeX(), std::max(0.0f, std::ceil(gameView.GetRect().Right)));

	// TODO: Check what parts the player can see!
	for (::Game::Map::SizeType y = beginY; y < endY; ++y) {
		for (::Game::Map::SizeType x = beginX; x < endX; ++x) {
			sf::Sprite sprite(images.get(map(x, y).texture));
			sprite.Resize(1, 1);
			sprite.SetPosition(x, y);
			window.Draw(sprite);
		}
	}

	// TODO: Get only visible objects! Maybe use something like game.forEachObject(rectangle, callback).
	const ::Game::Game::ObjectContainerType& objects = game.getObjects();
	for (::Game::Game::ObjectContainerType::const_iterator i = objects.begin(); i != objects.end(); ++i) {
		boost::shared_ptr<Object> object(getObject(i->second));
		object->draw(window, selectedObjects.find(object) != selectedObjects.end());
	}
}

void GUI::Game::Game::exit() {
	if (GUI::currentWidget.get() == this) {
		GUI::currentWidget.reset();
	}
}

bool GUI::Game::Game::handleEvent(const sf::Event& e, const sf::RenderWindow& window) {
	if (settingsMenu) {
		bool ret = settingsMenu->handleEvent(e, window);
		if (!settingsMenu->isOpen()) {
			settingsMenu.reset();
		}
		return ret;
	}
	if (e.Type == sf::Event::KeyPressed && e.Key.Code == sf::Key::Escape) {
		exit();
		return true;
	}

	// Update mouse position.
	if (e.Type == sf::Event::MouseButtonPressed || e.Type == sf::Event::MouseButtonReleased) {
		mouse.update(window, gameView, e.MouseButton.X, e.MouseButton.Y);
	} else if (e.Type == sf::Event::MouseMoved) {
		mouse.update(window, gameView, e.MouseMove.X, e.MouseMove.Y);
	}

	// Handle buttons etc.
	if (Container::handleEvent(e, window)) {
		return true;
	}

	// Store mouse position when button is pressed.
	if (e.Type == sf::Event::MouseButtonPressed) {
		mouseDownPosition[e.MouseButton.Button] = mouse.getPosition();
	}

	// Handle release only if the press was stored.
	if (e.Type == sf::Event::MouseButtonReleased && mouseDownPosition.find(e.MouseButton.Button) != mouseDownPosition.end()) {
		// Extract and erase the remembered position.
		Vector2<SIUnit::Position> oldPosition = mouseDownPosition[e.MouseButton.Button];
		mouseDownPosition.erase(e.MouseButton.Button);

		if (e.MouseButton.Button == sf::Mouse::Left) {
			// Select units (add to old selection if holding ctrl).
			bool ctrl = window.GetInput().IsKeyDown(sf::Key::LControl) || window.GetInput().IsKeyDown(sf::Key::RControl);
			if (!ctrl) {
				selectedObjects.clear();
			}
			ObjectSetType objects = getObjectsWithinRange(mouse.getPosition(), Scalar<SIUnit::Length>(0.15), 1);

			if (objects.size() > 0) {
				if (ctrl && selectedObjects.find(*objects.begin()) != selectedObjects.end()) {
					selectedObjects.erase(*objects.begin());
					return true;
				}
				selectedObjects.insert(*objects.begin());
				return true;
			}
		}
		// Command units only if the mouse hasn't moved much.
		if (e.MouseButton.Button == sf::Mouse::Right && (mouse.getPosition() - oldPosition).pow2() < 0.2) {
			//Testing movement
			::Game::Message msg;
			msg.action = ::Game::ObjectAction::MOVE;
			msg.position = mouse.getPosition();
			for (ObjectSetType::const_iterator i = selectedObjects.begin(); i != selectedObjects.end(); ++i) {
				msg.actors.push_back((*i)->getObject()->id);
			}
			connection->sendMessage(msg);
			return true;
		}
	}

	return false;
}

void GUI::Game::Game::updateState(sf::RenderWindow& window) {
	connection->runUntilNow();

	if (!settingsMenu) {
		gameView.update(window);
		mouse.update(window, gameView);
	}
}

void GUI::Game::Game::draw(sf::RenderWindow& window) {
	window.Clear();

	if (settingsMenu) {
		settingsMenu->draw(window);
	} else {
		window.SetView(gameView);
		drawGame(window);
		window.SetView(guiView);
		Container::draw(window);
	}
}

void GUI::Game::Game::openSettingsMenu(sf::RenderWindow& window) {
	settingsMenu.reset(new Menu::SettingsMenu(window));
}

boost::shared_ptr<GUI::Game::Object> GUI::Game::Game::getObject(const boost::shared_ptr<const ::Game::Object>& object) const {
	const void* key = object.get();
	ObjectMapType::iterator i = objects.find(key);

	if (i == objects.end()) {
		objects[key].reset(new Object(object));
		return objects[key];
	}

	return i->second;
}

GUI::Game::Game::ObjectSetType GUI::Game::Game::getObjectsWithinRange(Vector2<SIUnit::Position> position, Scalar<SIUnit::Length> range, int howMany) {
	const ::Game::Game& game = connection->getGame();
	const ::Game::Game::ObjectContainerType& objects = game.getObjects();

	ObjectSetType result;
	for (::Game::Game::ObjectContainerType::const_iterator i = objects.begin(); i != objects.end(); ++i) {
		if (i->second->isNear(position, range)) {
			result.insert(getObject(i->second));
			if (howMany && !--howMany) {
				break;
			}
		}
	}

	return result;
}

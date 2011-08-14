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

#include "gui/GUI.hpp"
#include "util/Path.hpp"

#include "GameHandler.hpp"
#include "GameObject.hpp"
#include "game/Map.hpp"

#include "gui/widget/Button.hpp"

#include <SFML/Graphics.hpp>

#include <algorithm>
#include <cmath>
#include <boost/bind.hpp>

GUI::GameHandler::GameHandler(boost::shared_ptr<GameConnection> connection_, sf::RenderWindow& window):
	guiView(window.GetDefaultView()),
	connection(connection_),
	gameView(window, sf::Vector2f(connection->getGame().getMap().getSizeX(), connection->getGame().getMap().getSizeY()), 32) {

	loadMapData();

	// TODO: Center at the player start position or something.
	gameView.SetCenter(
		connection->getGame().getMap().getSizeX() / 2,
		connection->getGame().getMap().getSizeY() / 2
	);

	insert(new GUI::Widget::Button("X", window.GetWidth() - 24, 0, 24, 24, boost::bind(&GUI::GameHandler::exit, this)));
	insert(new GUI::Widget::Button("S", window.GetWidth() - 48, 0, 24, 24, boost::bind(&GUI::GameHandler::openSettingsMenu, this, boost::ref(window))));
}

void GUI::GameHandler::loadMapData() {
	const Map& map = connection->getGame().getMap();
	const Map::TileInfoMap& tileInfoMap = map.getTileInfoMap();

	for (Map::TileInfoMap::const_iterator i = tileInfoMap.begin(); i != tileInfoMap.end(); ++i) {
		const Map::TileInfo& info = i->second;
		images.get(info.texture, Path::findDataPath(map.getDirectory(), "", info.texture));
	}
}

void GUI::GameHandler::drawGame(sf::RenderWindow& window) const {
	const Game& game = connection->getGame();
	const Map& map = game.getMap();

	//calculate which tiles are on the screen.
	Map::SizeType beginY = std::max(0.0f, gameView.GetRect().Top);
	Map::SizeType endY = std::min<Map::SizeType>(map.getSizeY(), std::max(0.0f, std::ceil(gameView.GetRect().Bottom)));
	Map::SizeType beginX = std::max(0.0f, gameView.GetRect().Left);
	Map::SizeType endX = std::min<Map::SizeType>(map.getSizeX(), std::max(0.0f, std::ceil(gameView.GetRect().Right)));

	// TODO: Check what parts the player can see!
	for (Map::SizeType y = beginY; y < endY; ++y) {
		for (Map::SizeType x = beginX; x < endX; ++x) {
			sf::Sprite sprite(images.get(map(x, y).texture));
			sprite.Resize(1, 1);
			sprite.SetPosition(x, y);
			window.Draw(sprite);
		}
	}

	//draw selection indicators.
	for (ObjectListType::const_iterator i = selectedObjects.begin(); i != selectedObjects.end(); ++i) {
		Vector2<SIUnit::Position> pos = i->get()->getObject()->getPosition();
		window.Draw(sf::Shape::Circle(pos.x.getDouble(), pos.y.getDouble(), 0.35f, sf::Color(128, 128, 255, 96), 0.1f, sf::Color(192, 192, 255)));
	}

	// TODO: Get only visible objects! Maybe use something like game.forEachObject(rectangle, callback).
	const Game::ObjectContainerType& objects = game.getObjects();
	for (Game::ObjectContainerType::const_iterator i = objects.begin(); i != objects.end(); ++i) {
		// TODO: Make the GameObjects persistent so that they can keep track of the animation!
		GameObject tmp(i->second);
		tmp.draw(window);
	}
}

void GUI::GameHandler::exit() {
	if (GUI::currentWidget.get() == this) {
		GUI::currentWidget.reset();
	}
}

bool GUI::GameHandler::handleEvent(const sf::Event& e, const sf::RenderWindow& window) {
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
	if (Container::handleEvent(e, window)) {
		return true;
	}

	if (e.Type == sf::Event::MouseButtonPressed) {
		if (e.MouseButton.Button == sf::Mouse::Right) {
			//Testing movement
			sf::Vector2f mousePosition = window.ConvertCoords(e.MouseButton.X, e.MouseButton.Y, &gameView);
			Message msg;
			msg.position.x = mousePosition.x;
			msg.position.y = mousePosition.y;
			for (ObjectListType::const_iterator i = selectedObjects.begin(); i != selectedObjects.end(); ++i) {
				msg.actors.push_back((*i)->getObject()->id);
			}
			connection->sendMessage(msg);
		}
	}

	if (e.Type == sf::Event::MouseButtonReleased) {
		if (e.MouseButton.Button == sf::Mouse::Left) {
			//Select units
			//TODO: Select multiple units.
			selectedObjects.clear();
			sf::Vector2f mousePosition = window.ConvertCoords(e.MouseButton.X, e.MouseButton.Y, &gameView);
			ObjectListType objects = getObjectsWithinRange(mousePosition.x, mousePosition.y, 1);

			if (objects.size() > 0) {
				selectedObjects.push_back(*objects.begin());
				return true;
			}
		}
	}

	return false;
}

void GUI::GameHandler::updateState(sf::RenderWindow& window) {
	connection->runUntilNow();

	if (!settingsMenu) {
		gameView.update(window);
	}
}

void GUI::GameHandler::draw(sf::RenderWindow& window) {
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

void GUI::GameHandler::openSettingsMenu(sf::RenderWindow& window) {
	settingsMenu.reset(new Menu::SettingsMenu(window));
}

boost::shared_ptr<GUI::GameObject> GUI::GameHandler::getGameObject(boost::shared_ptr<World::Object> object) {
	GameObjectListType::iterator i = gameObjects.find(object);

	if (i == gameObjects.end()) {
		gameObjects[object].reset(new GUI::GameObject(object));
		return gameObjects[object];
	}

	return i->second;
}

GUI::GameHandler::ObjectListType GUI::GameHandler::getObjectsWithinRange(float x, float y, float r) {
	const Game& game = connection->getGame();
	const Game::ObjectContainerType& objects = game.getObjects();

	ObjectListType result;

	for (Game::ObjectContainerType::const_iterator i = objects.begin(); i != objects.end(); ++i) {
		if (i->second->isNear(Vector2<SIUnit::Position>(x, y), Scalar<SIUnit::Length>(r))) {
			result.push_back(getGameObject(i->second));
		}
	}

	return result;
}

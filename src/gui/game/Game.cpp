#include <algorithm>
#include <cmath>
#include <functional>

#include "util/Path.hpp"

#include "game/Game.hpp"
#include "game/Map.hpp"
#include "connection/Client.hpp"

#include "gui/GUI.hpp"

#include "gui/game/Game.hpp"
#include "gui/game/Object.hpp"

#include "gui/widget/Button.hpp"

#include <SFML/Graphics.hpp>

GUI::Game::Game::Game(std::shared_ptr<Connection::Client> client_, sf::RenderWindow& window):
	guiView(window.getDefaultView()),
	client(client_),
	gameView(window, sf::Vector2f(client->getGame().getMap().getSizeX(), client->getGame().getMap().getSizeY()), 32) {

	loadMapData();

	// TODO: Center at the player start position or something.
	gameView.setCenter(
		client->getGame().getMap().getSizeX() / 2,
		client->getGame().getMap().getSizeY() / 2
	);

	insert(new GUI::Widget::Button("X", window.getSize().x - 24, 0, 24, 24, std::bind(&Game::exit, this)));
	insert(new GUI::Widget::Button("S", window.getSize().x - 48, 0, 24, 24, std::bind(&Game::openSettingsMenu, this, std::ref(window))));

	client->setReadyToStart();
}

void GUI::Game::Game::loadMapData() {
	const ::Game::Map& map = client->getGame().getMap();
	const ::Game::Map::TileInfoMap& tileInfoMap = map.getTileInfoMap();

	for (::Game::Map::TileInfoMap::const_iterator i = tileInfoMap.begin(); i != tileInfoMap.end(); ++i) {
		const ::Game::Map::TileInfo& info = i->second;
		textures.get(info.texture, Path::findDataPath(map.getDirectory(), "", info.texture));
	}
}

void GUI::Game::Game::drawGame(sf::RenderWindow& window) const {
	const ::Game::Game& game = client->getGame();
	const ::Game::Map& map = game.getMap();

	//calculate which tiles are on the screen.
	sf::Vector2f xy0 = gameView.getCenter() - gameView.getSize() * 0.5f;
	sf::Vector2f xy1 = xy0 + gameView.getSize();
	::Game::Map::SizeType beginY = std::max(0.0f, xy0.y);
	::Game::Map::SizeType endY = std::min< ::Game::Map::SizeType>(map.getSizeY(), std::max(0.0f, std::ceil(xy1.y)));
	::Game::Map::SizeType beginX = std::max(0.0f, xy0.x);
	::Game::Map::SizeType endX = std::min< ::Game::Map::SizeType>(map.getSizeX(), std::max(0.0f, std::ceil(xy1.x)));

	// TODO: Check what parts the player can see!
	for (::Game::Map::SizeType y = beginY; y < endY; ++y) {
		for (::Game::Map::SizeType x = beginX; x < endX; ++x) {
			sf::Sprite sprite(textures.get(map(x, y).texture));
			sprite.setScale(1 / sprite.getLocalBounds().width, 1 / sprite.getLocalBounds().height);
			sprite.setPosition(x, y);
			window.draw(sprite);
		}
	}

	// TODO: Get only visible objects! Maybe use something like game.forEachObject(rectangle, callback).
	const ::Game::Game::ObjectContainerType& objects = game.getObjects();
	for (::Game::Game::ObjectContainerType::const_iterator i = objects.begin(); i != objects.end(); ++i) {
		std::shared_ptr<Object> object(getObject(i->second));
		bool selected = selectedObjects.find(object) != selectedObjects.end();
		object->draw(window, client->getClientInfo(), selected);
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
	if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape) {
		exit();
		return true;
	}

	// Update mouse position.
	if (e.type == sf::Event::MouseButtonPressed || e.type == sf::Event::MouseButtonReleased) {
		mouse.update(window, gameView, e.mouseButton.x, e.mouseButton.y);
	} else if (e.type == sf::Event::MouseMoved) {
		mouse.update(window, gameView, e.mouseMove.x, e.mouseMove.y);
	}

	// Handle buttons etc.
	if (Container::handleEvent(e, window)) {
		return true;
	}

	// Store mouse position when button is pressed.
	if (e.type == sf::Event::MouseButtonPressed) {
		mouseDownPosition[e.mouseButton.button] = mouse.getPosition();
	}

	// Handle release only if the press was stored.
	if (e.type == sf::Event::MouseButtonReleased && mouseDownPosition.find(e.mouseButton.button) != mouseDownPosition.end()) {
		// Extract and erase the remembered position.
		Vector2<SIUnit::Position> oldPosition = mouseDownPosition[e.mouseButton.button];
		mouseDownPosition.erase(e.mouseButton.button);

		if (e.mouseButton.button == sf::Mouse::Left) {
			// Select units (add to old selection if holding ctrl).
			bool ctrl = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl);
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
		if (e.mouseButton.button == sf::Mouse::Right && (mouse.getPosition() - oldPosition).pow2() < 0.2) {
			//Testing movement
			::Game::Message msg;
			msg.action = ::Game::ObjectAction::MOVE;
			msg.position = mouse.getPosition();
			for (ObjectSetType::const_iterator i = selectedObjects.begin(); i != selectedObjects.end(); ++i) {
				msg.actors.push_back((*i)->getObject()->id);
			}
			client->sendMessage(msg);
			return true;
		}
	}

	// Pressing delete destroys any selected units.
	if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Delete && !selectedObjects.empty()) {
		::Game::Message msg;
		msg.action = ::Game::ObjectAction::DELETE;
		for (ObjectSetType::const_iterator i = selectedObjects.begin(); i != selectedObjects.end(); ++i) {
			msg.actors.push_back((*i)->getObject()->id);
		}
		client->sendMessage(msg);
		return true;
	}

	return false;
}

void GUI::Game::Game::updateState(sf::RenderWindow& window) {
	client->update();

	if (!settingsMenu) {
		gameView.update(window);
		mouse.update(window, gameView);
	}
}

void GUI::Game::Game::draw(sf::RenderWindow& window) {
	window.clear();

	if (settingsMenu) {
		settingsMenu->draw(window);
	} else {
		window.setView(gameView);
		drawGame(window);
		window.setView(guiView);
		Container::draw(window);
	}
}

void GUI::Game::Game::openSettingsMenu(sf::RenderWindow& window) {
	settingsMenu.reset(new Menu::SettingsMenu(std::shared_ptr<Widget>(), window));
}

std::shared_ptr<GUI::Game::Object> GUI::Game::Game::getObject(const std::shared_ptr<const ::Game::Object>& object) const {
	const void* key = object.get();
	ObjectMapType::iterator i = objects.find(key);

	if (i == objects.end()) {
		objects[key].reset(new Object(object));
		return objects[key];
	}

	return i->second;
}

GUI::Game::Game::ObjectSetType GUI::Game::Game::getObjectsWithinRange(Vector2<SIUnit::Position> position, Scalar<SIUnit::Length> range, int howMany) {
	const ::Game::Game& game = client->getGame();
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

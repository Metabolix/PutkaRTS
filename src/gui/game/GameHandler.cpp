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
#include "game/Map.hpp"

#include "gui/widget/Button.hpp"
#include "gui/widget/Container.hpp"

#include <SFML/Graphics.hpp>

#include <memory>
#include <algorithm>
#include <cmath>
#include <boost/bind.hpp>

std::auto_ptr<GUI::GameHandler> GUI::GameHandler::instance;

const int GUI::GameHandler::tileSize = 32;

GUI::GameHandler::GameHandler(std::auto_ptr<GameConnection> connection_):
	connection(connection_),
	mouseDrag(false) {
	instance.reset(this);
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
	const Map& map = connection->getGame().getMap();

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

	// TODO: Draw the objects as well!
}

void GUI::GameHandler::exit() {
	gameClosed = true;
}

void GUI::GameHandler::run(sf::RenderWindow& window) {
	guiView = window.GetDefaultView();

	resetGameView(window, true);

	window.SetFramerateLimit(60);

	loadMapData();

	Widget::Container gui;
	gui.insert(boost::shared_ptr<Widget::Button>(new Widget::Button("X", window.GetWidth() - 24, 0, 24, 24, boost::bind(&GUI::GameHandler::exit, this))));

	gameClosed = false;
	while (window.IsOpened() && !gameClosed) {
		sf::Event e;
		if (window.GetEvent(e)) {
			if (e.Type == sf::Event::Closed) {
				window.Close();
				break;
			}
			if (e.Type == sf::Event::KeyPressed && e.Key.Code == sf::Key::Escape) {
				break;
			}
			if (gui.handleEvent(e, window)) {
				continue;
			}
		} else {
			handleScrolling(window);

			window.Clear();

			window.SetView(gameView);
			drawGame(window);

			window.SetView(guiView);
			gui.draw(window);

			window.Display();
		}
	}
}

void GUI::GameHandler::handleScrolling(sf::RenderWindow& window) {
	float time = window.GetFrameTime();
	const sf::Input & input = window.GetInput();
	const Map& map = connection->getGame().getMap();

	const float zoomSpeed = GUI::config.getDouble("gameUI.zoomSpeed", 2);

	// zoom with pgUp and pgDown
	if (input.IsKeyDown(sf::Key::PageDown)) {
		gameView.Zoom(pow(2, zoomSpeed * time));
	} else if (input.IsKeyDown(sf::Key::PageUp)) {
		gameView.Zoom(pow(2, zoomSpeed * -time));
	}

	// limit zoom level to at least 4 tiles or at most map size x2
	sf::Vector2f halfSize = gameView.GetHalfSize();
	if (halfSize.x < 2) {
		halfSize *= 2 / halfSize.x;
	}
	if (halfSize.y < 2) {
		halfSize *= 2 / halfSize.y;
	}
	if (halfSize.x > map.getSizeX()) {
		halfSize *= map.getSizeX() / halfSize.x;
	}
	if (halfSize.y > map.getSizeY()) {
		halfSize *= map.getSizeY() / halfSize.y;
	}
	gameView.SetHalfSize(halfSize);

	const float keyboardScrollSpeed = 2 * halfSize.x * GUI::config.getDouble("gameUI.keyboardScrollSpeed", 1);
	const float borderScrollSpeed = 2 * halfSize.x * GUI::config.getDouble("gameUI.borderScrollSpeed", 1);
	const bool reverseDrag = GUI::config.getBool("gameUI.reverseDrag", false);

	// scroll map with arrow keys
	if (input.IsKeyDown(sf::Key::Right)) {
		gameView.Move(keyboardScrollSpeed * time, 0);
	} else if (input.IsKeyDown(sf::Key::Left)) {
		gameView.Move(-keyboardScrollSpeed * time, 0);
	}
	if (input.IsKeyDown(sf::Key::Down)) {
		gameView.Move(0, keyboardScrollSpeed * time);
	} else if (input.IsKeyDown(sf::Key::Up)) {
		gameView.Move(0, -keyboardScrollSpeed * time);
	}

	// mouse scrolling
	int threshold = 5;

	if (input.GetMouseX() < threshold) {
		gameView.Move(-borderScrollSpeed * time, 0);
	} else if (input.GetMouseX() > (int) window.GetWidth() - threshold) {
		gameView.Move(borderScrollSpeed * time, 0);
	}
	if (input.GetMouseY() < threshold) {
		gameView.Move(0, -borderScrollSpeed * time);
	} else if (input.GetMouseY() > (int) window.GetHeight() - threshold) {
		gameView.Move(0, borderScrollSpeed * time);
	}

	// drag with right mouse
	if (input.IsMouseButtonDown(sf::Mouse::Right)) {
		if (mouseDrag) {
			sf::Vector2f mapDragOrigin = window.ConvertCoords(dragOrigin.x, dragOrigin.y, &gameView);
			sf::Vector2f mapDragDestination = window.ConvertCoords(input.GetMouseX(), input.GetMouseY(), &gameView);
			sf::Vector2f change(mapDragOrigin - mapDragDestination);
			if (reverseDrag) {
				change = -change;
			}
			gameView.Move(change);
		}
		if (mouseDrag) {
			window.SetCursorPosition(dragOrigin.x, dragOrigin.y);
		} else {
			dragOrigin = sf::Vector2f(input.GetMouseX(), input.GetMouseY());
		}
		mouseDrag = true;
	} else {
		mouseDrag = false;
	}

	//reset view with home key.
	if (input.IsKeyDown(sf::Key::Home)) {
		resetGameView(window, false);
	}

	// boundaries
	sf::Vector2f viewCenter = gameView.GetCenter();
	viewCenter.x = std::max<float>(0.0f, std::min<float>(viewCenter.x, map.getSizeX()));
	viewCenter.y = std::max<float>(0.0f, std::min<float>(viewCenter.y, map.getSizeY()));
	gameView.SetCenter(viewCenter);
}

void GUI::GameHandler::resetGameView(sf::RenderWindow& window, bool resetLocation) {
	sf::Vector2f location = gameView.GetCenter();
	if (resetLocation) {
		// TODO: Center at the player start position or something.
		location.x = connection->getGame().getMap().getSizeX() / 2;
		location.y = connection->getGame().getMap().getSizeY() / 2;
	}
	gameView = window.GetDefaultView();
	gameView.SetCenter(location);
	gameView.Zoom(tileSize);
}

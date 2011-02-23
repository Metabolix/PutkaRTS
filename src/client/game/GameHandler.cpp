/*
 * Client-side game handler class implementation.
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

#include "util/Path.hpp"

#include "GameHandler.hpp"
#include "game/Map.hpp"

#include <SFML/Graphics.hpp>

#include <memory>
#include <algorithm>
#include <cmath>

std::auto_ptr<GameHandler> GameHandler::instance;

const int GameHandler::tileSize = 32;

GameHandler::GameHandler(std::auto_ptr<GameConnection> connection_):
	connection(connection_) {
	instance.reset(this);
}

void GameHandler::loadMapData() {
	const Map& map = connection->getGame().getMap();
	const Map::TileInfoMap& tileInfoMap = map.getTileInfoMap();

	for (Map::TileInfoMap::const_iterator i = tileInfoMap.begin(); i != tileInfoMap.end(); ++i) {
		const Map::TileInfo& info = i->second;
		images.get(info.texture, Path::findDataPath(map.getDirectory(), "", info.texture));
	}
}

void GameHandler::drawGame(sf::RenderWindow& window) const {
	const Map& map = connection->getGame().getMap();

	window.SetView(gameView);

	//calculate which tiles are on the screen.
	Map::SizeType beginY = std::max<int>(0, gameView.GetRect().Top / tileSize);
	Map::SizeType endY = std::min<int>(map.getSizeY(), std::max<int>(0, std::ceil(gameView.GetRect().Bottom / tileSize)));
	Map::SizeType beginX = std::max<int>(0, gameView.GetRect().Left / tileSize);
	Map::SizeType endX = std::min<int>(map.getSizeX(), std::max<int>(0, std::ceil(gameView.GetRect().Right / tileSize)));

	// TODO: Check what parts the player can see!
	for (Map::SizeType y = beginY; y < endY; ++y) {
		for (Map::SizeType x = beginX; x < endX; ++x) {
			sf::Sprite sprite(images.get(map(x, y).texture));
			sprite.Resize(tileSize, tileSize);
			sprite.SetPosition(x * tileSize, y * tileSize);
			window.Draw(sprite);
		}
	}

	// TODO: Draw the objects as well!

	window.SetView(view);
}

void GameHandler::run(sf::RenderWindow& window) {
	view = window.GetDefaultView();
	window.SetView(view);

	gameView = window.GetView();
	gameView.SetCenter(
		sf::Vector2f(
			connection->getGame().getMap().getSizeX() * tileSize / 2,
			connection->getGame().getMap().getSizeY() * tileSize / 2
		)
	);

	window.SetFramerateLimit(60);

	loadMapData();

	while (window.IsOpened()) {
		sf::Event e;
		while (window.GetEvent(e)) {
			if (e.Type == sf::Event::Closed || (e.Type == sf::Event::KeyPressed && e.Key.Code == sf::Key::Escape)) {
				window.Close();
				return;
			}

			if (e.Type == sf::Event::MouseButtonPressed) {
				if (e.MouseButton.Button == sf::Mouse::Left) {
					return;
				}
			}
		}

		handleScrolling(window);

		window.Clear();

		drawGame(window);

		// TODO: Draw the user interface!
		window.Draw(sf::String("Game; click to return to menu."));

		window.Display();
	}
}

void GameHandler::handleScrolling(sf::RenderWindow& window) {
	float time = window.GetFrameTime();
	const sf::Input & input = window.GetInput();

	static float scrollSpeed = 400.0f;
	static float zoomSpeed = 2.0f;

	// scroll map with arrow keys
	if (input.IsKeyDown(sf::Key::Right)) {
		gameView.Move(scrollSpeed * time, 0);
	} else if (input.IsKeyDown(sf::Key::Left)) {
		gameView.Move(-scrollSpeed * time, 0);
	}
	if (input.IsKeyDown(sf::Key::Down)) {
		gameView.Move(0, scrollSpeed * time);
	} else if (input.IsKeyDown(sf::Key::Up)) {
		gameView.Move(0,-scrollSpeed * time);
	}
	//zoom with pgUp and pgDown
	if (input.IsKeyDown(sf::Key::PageDown)) {
		float zoomAmount = pow(zoomSpeed, time);
		gameView.Zoom(zoomAmount);
		scrollSpeed *= 1.00f / zoomAmount;
	} else if (input.IsKeyDown(sf::Key::PageUp)) {
		float zoomAmount = pow(zoomSpeed, time);
		gameView.Zoom(1.00f / zoomAmount);
		scrollSpeed *= zoomAmount;
	}

	//mouse scrolling
	int threshold = 5;

	if (input.GetMouseX() < threshold) {
		gameView.Move(-scrollSpeed * time, 0);
	} else if (input.GetMouseX() > window.GetWidth() - threshold) {
		gameView.Move(scrollSpeed * time, 0);
	}
	if (input.GetMouseY() < threshold) {
		gameView.Move(0, -scrollSpeed * time);
	} else if (input.GetMouseY() > window.GetHeight() - threshold) {
		gameView.Move(0, scrollSpeed * time);
	}

	//drag with right mouse
	static bool mouseDrag = false;
	static sf::Vector2f dragOrigin;

	if (input.IsMouseButtonDown(sf::Mouse::Right)) {
		if (mouseDrag) {
			gameView.Move(input.GetMouseX() - dragOrigin.x, input.GetMouseY() - dragOrigin.y);
		} else {
			dragOrigin = sf::Vector2f(input.GetMouseX(), input.GetMouseY());
		}

		window.SetCursorPosition(dragOrigin.x, dragOrigin.y);
		mouseDrag = true;
	} else {
		mouseDrag = false;
	}

	//boundaries
	sf::Vector2f viewCenter = gameView.GetCenter();
	const Map& map = connection->getGame().getMap();
	viewCenter.x = std::max<float>(0.0f, std::min<float>(viewCenter.x, map.getSizeX() * tileSize));
	viewCenter.y = std::max<float>(0.0f, std::min<float>(viewCenter.y, map.getSizeY() * tileSize));
	gameView.SetCenter(viewCenter);
}

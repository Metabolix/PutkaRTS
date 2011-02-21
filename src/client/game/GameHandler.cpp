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

std::auto_ptr<GameHandler> GameHandler::instance;

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
	const int tileSize = 32;

	window.SetView(gameView);

	//calculate which tiles are on the screen.
	Map::SizeType beginY = std::max(0 , (int)(gameView.GetRect().Top / tileSize));
	Map::SizeType endY = std::min((int)map.getSizeY(), (int)((gameView.GetRect().Bottom) / tileSize) + 1);
	Map::SizeType beginX = std::max(0, (int)(gameView.GetRect().Left / tileSize));
	Map::SizeType endX = std::min((int)map.getSizeX(), (int)((gameView.GetRect().Right) / tileSize) + 1);

	// TODO: Check what parts the player can see!
	for (Map::SizeType y = beginY; y < endY; ++y) {
		for (Map::SizeType x = beginX; x < endX; ++x) {
			sf::Sprite sprite(images.get(map(x, y).texture));
			sprite.Resize(tileSize, tileSize);
			sprite.SetPosition((int)x * tileSize,  (int)y * tileSize);
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
	gameView.SetCenter(gameView.GetHalfSize());

	window.SetFramerateLimit(0);

	loadMapData();

	while (window.IsOpened()) {
		sf::Event e;
		while (window.GetEvent(e)) {
			if (e.Type == sf::Event::Closed || (e.Type == sf::Event::KeyPressed && e.Key.Code == sf::Key::Escape)) {
				window.Close();
				return;
			}

			if (e.Type == sf::Event::MouseButtonPressed) {
				return;
			}
		}

		const sf::Input & input = window.GetInput();

		// scroll map with arrow keys
		if (input.IsKeyDown(sf::Key::Right)) {
			gameView.Move(5, 0);
		} else if (input.IsKeyDown(sf::Key::Left)) {
			gameView.Move(-5, 0);
		}
		if (input.IsKeyDown(sf::Key::Down)) {
			gameView.Move(0, 5);
		} else if (input.IsKeyDown(sf::Key::Up)) {
			gameView.Move(0,-5);
		}
		if (input.IsKeyDown(sf::Key::PageDown)) {
			gameView.Zoom(1.02f);
		} else if (input.IsKeyDown(sf::Key::PageUp)) {
			gameView.Zoom(1.00f / 1.02f);
		}

		window.Clear();

		drawGame(window);

		// TODO: Draw the user interface!
		window.Draw(sf::String("Game; click to return to menu."));

		window.Display();
		sf::Sleep(0.025f);
	}
}

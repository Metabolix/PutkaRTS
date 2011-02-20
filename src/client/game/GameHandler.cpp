/*
 * Client-side game handler class implementation.
 *
 * Copyright 2011 Lauri Kenttä
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

	// TODO: Implement scrolling and draw only the part that fits the screen!
	// TODO: Check what parts the player can see!
	for (Map::SizeType y = 0; y < map.getSizeY(); ++y) {
		for (Map::SizeType x = 0; x < map.getSizeX(); ++x) {
			sf::Sprite sprite(images.get(map(x, y).texture));
			const int tileSize = 32;
			sprite.Resize(tileSize, tileSize);
			sprite.SetPosition(x * tileSize, y * tileSize);
			window.Draw(sprite);
		}
	}

	// TODO: Draw the objects as well!
}

void GameHandler::run(sf::RenderWindow& window) {
	view = window.GetDefaultView();
	window.SetView(view);

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

		window.Clear();

		drawGame(window);

		// TODO: Draw the user interface!
		window.Draw(sf::String("Game; click to return to menu."));

		window.Display();
		sf::Sleep(0.1f);
	}
}

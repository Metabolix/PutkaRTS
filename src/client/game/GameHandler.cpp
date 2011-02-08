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

#include "GameHandler.hpp"
#include <sstream>
#include <memory>

std::auto_ptr<GameHandler> GameHandler::instance;

GameHandler::GameHandler(std::auto_ptr<GameConnection> connection_):
	connection(connection_) {
	instance.reset(this);
}

void GameHandler::run(sf::RenderWindow& window) {
	sf::Clock clock;
	while (window.IsOpened()) {
		sf::Event e;
		while (window.GetEvent(e)) {
			if (e.Type == sf::Event::Closed) {
				window.Close();
				return;
			}

			if (e.Type == sf::Event::MouseButtonPressed) {
				return;
			}
		}
		std::ostringstream ost;
		ost << "This is the game mode.\nClick to return to menu.";
		window.Clear();
		window.Draw(sf::String(ost.str()));
		window.Display();
		sf::Sleep(0.1f);
	}
	window.Close();
}

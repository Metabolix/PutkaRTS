/*
 * Main menu class implementation.
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

#include "MainMenuHandler.hpp"
#include "client/game/GameHandler.hpp"
#include "client/game/LocalGameConnection.hpp"
#include "game/Game.hpp"

#include <sstream>
#include <memory>

void MainMenuHandler::startGame() {
	std::auto_ptr<Game> game(new Game());
	std::auto_ptr<GameConnection> connection(new LocalGameConnection(game));
	GameHandler::instance.reset(new GameHandler(connection));
}

void MainMenuHandler::run(sf::RenderWindow& window) {
	GameHandler::instance.reset();

	while (window.IsOpened()) {
		sf::Event e;
		while (window.GetEvent(e)) {
			if (e.Type == sf::Event::Closed) {
				window.Close();
				return;
			}

			if (e.Type == sf::Event::MouseButtonPressed) {
				startGame();
				return;
			}
		}
		std::ostringstream ost;
		ost << "This is the menu.\nClick to enter game.";
		window.Clear();
		window.Draw(sf::String(ost.str()));
		window.Display();
		sf::Sleep(0.1f);
	}
}

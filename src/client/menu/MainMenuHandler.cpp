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

#include "client/graphics/ImageCache.hpp"

#include <memory>
#include <algorithm>

void MainMenuHandler::startGame() {
	std::auto_ptr<Map> map(new Map("maps/testmap"));
	std::auto_ptr<Game> game(new Game(map));
	std::auto_ptr<GameConnection> connection(new LocalGameConnection(game));
	GameHandler::instance.reset(new GameHandler(connection));
	menuClosed = true;
}

void MainMenuHandler::run(sf::RenderWindow& window) {
	ImageCache mainMenuImages;
	const sf::Image& logoImage = mainMenuImages.get("data/graphics/logo.png");

	// Entering main menu, no game should be running.
	GameHandler::instance.reset();

	// Make view that is as close to 640x480 as possible and centered.
	view = window.GetDefaultView();
	view.Zoom(std::min(view.GetRect().GetWidth() / 640, view.GetRect().GetHeight() / 480));
	view.SetCenter(view.GetHalfSize());
	window.SetView(view);

	// Position at the top of the window.
	sf::Sprite logoSprite(logoImage);
	logoSprite.SetCenter(logoImage.GetWidth() / 2, 0);
	logoSprite.SetPosition(window.GetView().GetRect().GetWidth() / 2, 1);

	window.SetFramerateLimit(30);

	menuClosed = false;
	while (window.IsOpened() && !menuClosed) {
		sf::Event e;
		if (window.GetEvent(e)) {
			if (e.Type == sf::Event::Closed || (e.Type == sf::Event::KeyPressed && e.Key.Code == sf::Key::Escape)) {
				window.Close();
				continue;
			}

			if (e.Type == sf::Event::MouseButtonPressed) {
				startGame();
				continue;
			}
		} else {
			window.Clear(sf::Color(0xcc, 0x66, 0x33));
			window.Draw(logoSprite);
			window.Draw(sf::String("Menu; click to start game."));
			window.Display();
		}
	}
}

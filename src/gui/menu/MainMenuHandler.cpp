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
#include "gui/game/GameHandler.hpp"
#include "game/LocalGameConnection.hpp"
#include "game/Game.hpp"

#include "gui/widget/Button.hpp"

#include "util/Path.hpp"

#include <memory>
#include <algorithm>
#include <boost/bind.hpp>

GUI::MainMenuHandler::MainMenuHandler(sf::RenderWindow& window) {
	// Put the logo at the top of the window.
	const sf::Image& logoImage(images.get(Path::findDataPath("graphics/logo.png")));
	logoSprite.SetImage(logoImage);
	logoSprite.SetCenter(logoImage.GetWidth() / 2, 0);
	logoSprite.SetPosition(320, 1);

	// Build the main menu GUI.
	insert(new GUI::Widget::Button("New game", 200, 100, 240, 50, boost::bind(&GUI::MainMenuHandler::startGame, this, boost::ref(window))));
	insert(new GUI::Widget::Button("Exit", 250, 170, 140, 50, boost::bind(&sf::RenderWindow::Close, boost::ref(window))));
}

void GUI::MainMenuHandler::startGame(sf::RenderWindow& window) {
	std::auto_ptr<Map> map(new Map("maps/testmap"));
	std::auto_ptr<Game> game(new Game(map));
	std::auto_ptr<GameConnection> connection(new LocalGameConnection(game));
	GameHandler::instance.reset(new GameHandler(connection, window));
}

void GUI::MainMenuHandler::draw(sf::RenderWindow& window) {
	// Make view that is as close to 640x480 as possible and centered.
	view = window.GetDefaultView();
	view.Zoom(std::min(view.GetRect().GetWidth() / 640, view.GetRect().GetHeight() / 480));
	view.SetCenter(320, 240);
	window.SetView(view);

	window.Clear(sf::Color(0xcc, 0x66, 0x33));
	window.Draw(logoSprite);
	Container::draw(window);
	window.Display();
}

void GUI::MainMenuHandler::run(sf::RenderWindow& window) {
	// Entering main menu, no game should be running.
	GameHandler::instance.reset();

	window.SetFramerateLimit(30);

	while (window.IsOpened() && !GameHandler::instance.get()) {
		sf::Event e;
		if (window.GetEvent(e)) {
			if (e.Type == sf::Event::Closed || (e.Type == sf::Event::KeyPressed && e.Key.Code == sf::Key::Escape)) {
				window.Close();
				continue;
			}
			handleEvent(e, window);
		} else {
			draw(window);
		}
	}
}

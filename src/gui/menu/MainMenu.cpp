/*
 * Main menu class implementation.
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

#include "MainMenu.hpp"


#include "gui/GUI.hpp"
#include "gui/widget/Button.hpp"
#include "gui/menu/SettingsMenu.hpp"
#include "gui/game/GameHandler.hpp"

#include "game/LocalGameConnection.hpp"
#include "game/Game.hpp"

#include <memory>
#include <algorithm>
#include <boost/bind.hpp>

GUI::Menu::MainMenu::MainMenu(sf::RenderWindow& window):
	Menu(window) {
	// Build the main menu GUI.
	insert(new GUI::Widget::Button("New game", 200, 100, 240, 50, boost::bind(&GUI::Menu::MainMenu::startGame, this, boost::ref(window))));
	insert(new GUI::Widget::Button("Exit", 250, 240, 140, 50, boost::bind(&sf::RenderWindow::Close, boost::ref(window))));
}

void GUI::Menu::MainMenu::startGame(sf::RenderWindow& window) {
	std::auto_ptr<Map> map(new Map("maps/testmap"));
	std::auto_ptr<Game> game(new Game(map));
	std::auto_ptr<GameConnection> connection(new LocalGameConnection(game));
	GUI::currentWidget.reset(new GameHandler(connection, window));
}

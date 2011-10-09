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

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/thread.hpp>

#include "connection/Server.hpp"
#include "connection/Client.hpp"
#include "connection/Address.hpp"

#include "MainMenu.hpp"

#include "gui/GUI.hpp"
#include "gui/widget/Button.hpp"
#include "gui/menu/SettingsMenu.hpp"
#include "gui/menu/StartGame.hpp"
#include "gui/menu/JoinGame.hpp"

GUI::Menu::MainMenu::MainMenu(sf::RenderWindow& window):
	Menu() {
	// Build the main menu GUI.
	insert(new GUI::Widget::Button("New game", 200, 100 + 0 * 70, 240, 50, boost::bind(&GUI::Menu::MainMenu::startGame, this, boost::ref(window))));
	insert(new GUI::Widget::Button("Connect",  200, 100 + 1 * 70, 240, 50, boost::bind(&GUI::Menu::MainMenu::startMultiGame, this, boost::ref(window))));
	insert(new GUI::Widget::Button("Settings", 250, 100 + 2 * 70, 140, 50, boost::bind(&GUI::Menu::MainMenu::gotoSettings, this, boost::ref(window))));
	insert(new GUI::Widget::Button("Exit",     250, 100 + 3 * 70, 140, 50, boost::bind(&sf::RenderWindow::Close, boost::ref(window))));
}

void GUI::Menu::MainMenu::startMultiGame(sf::RenderWindow& window) {
	GUI::currentWidget.reset(new GUI::Menu::JoinGame(GUI::currentWidget));
}

void GUI::Menu::MainMenu::startGame(sf::RenderWindow& window) {
	boost::shared_ptr<Connection::Server> server(new Connection::Server());
	boost::shared_ptr<Connection::Client> client(server->createLocalClient());
	boost::thread(boost::bind(&Connection::Server::run, server));
	GUI::currentWidget.reset(new GUI::Menu::StartGame(GUI::currentWidget, client));
}

void GUI::Menu::MainMenu::gotoSettings(sf::RenderWindow& window) {
	GUI::currentWidget.reset(new GUI::Menu::SettingsMenu(GUI::currentWidget, window));
}

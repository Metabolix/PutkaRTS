/*
 * Dialog for starting a game.
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

#include <functional>

#include "StartGame.hpp"

#include "gui/GUI.hpp"
#include "gui/widget/Button.hpp"
#include "gui/game/Game.hpp"

GUI::Menu::StartGame::StartGame(std::shared_ptr<Widget> parent_, std::shared_ptr<Connection::Client> client_):
	Menu(parent_),
	client(client_) {

	insert(new GUI::Widget::Button("Ready", 400, 300, 100, 30, std::bind(&Connection::Client::setReadyToInit, client)));
}

void GUI::Menu::StartGame::updateState(sf::RenderWindow& window) {
	Menu::updateState(window);
	client->update();
	if (client->getState() >= Connection::INIT) {
		GUI::currentWidget.reset(new GUI::Game::Game(client, window));
	}
}

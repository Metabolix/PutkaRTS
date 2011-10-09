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

#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include "connection/Client.hpp"
#include "connection/Address.hpp"

#include "gui/GUI.hpp"
#include "gui/widget/Button.hpp"
#include "gui/game/Game.hpp"
#include "gui/menu/JoinGame.hpp"
#include "gui/menu/StartGame.hpp"

GUI::Menu::JoinGame::JoinGame(boost::shared_ptr<Widget> parent_):
	Menu(parent_),
	gameList(new GUI::Widget::List(100, 200, 200, 200, boost::bind(&GUI::Menu::JoinGame::selectGame, this, _1))),
	selectedId(0),
	statusLabel(new GUI::Widget::Label("", 100, 100, 440, 100)) {
	insert(gameList);
	insert(statusLabel);
	insert(new GUI::Widget::Button("Join", 400, 300, 100, 30, boost::bind(&GUI::Menu::JoinGame::joinGame, this)));
	insert(new GUI::Widget::Button("Cancel", 400, 340, 100, 30, boost::bind(&GUI::Menu::JoinGame::closeMenu, this)));
}

void GUI::Menu::JoinGame::updateState(sf::RenderWindow& window) {
	Menu::updateState(window);
	try {
		if (!metaserver.getGames()) {
			return;
		}
	} catch (std::runtime_error& e) {
		statusLabel->setText(e.what());
		return;
	}
	gameList->clear();

	typedef std::pair<const int, Connection::Metaserver::Game> PairType;
	BOOST_FOREACH(const PairType& pair, metaserver.games) {
		const Connection::Metaserver::Game& game(pair.second);
		gameList->insertItem(boost::lexical_cast<std::string>(game.id), game.name);
	}
	gameList->selectItem(boost::lexical_cast<std::string>(selectedId));
}

void GUI::Menu::JoinGame::selectGame(const GUI::Widget::List::Item& item) {
	selectedId = item.key.empty() ? 0 : boost::lexical_cast<int>(item.key);
}

void GUI::Menu::JoinGame::joinGame() {
	if (metaserver.games.find(selectedId) == metaserver.games.end()) {
		return; // Invalid selection.
	}
	boost::shared_ptr<Connection::EndPoint> connection;
	std::string error;
	BOOST_FOREACH(const std::string& address, metaserver.games[selectedId].addresses) {
		try {
			connection = Connection::Address::connect(address);
			break;
		} catch (std::runtime_error& e) {
			if (!error.empty()) {
				error += "\n";
			}
			error += e.what();
		}
	}
	if (!connection) {
		statusLabel->setText(error);
	} else {
		boost::shared_ptr<Connection::Client> client(new Connection::Client(connection));
		GUI::currentWidget.reset(new GUI::Menu::StartGame(GUI::currentWidget, client));
	}
}

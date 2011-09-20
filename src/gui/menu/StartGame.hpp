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

#ifndef PUTKARTS_GUI_Menu_StartGame_HPP
#define PUTKARTS_GUI_Menu_StartGame_HPP

#include "connection/Client.hpp"

#include "gui/menu/Menu.hpp"

#include <SFML/Graphics.hpp>

namespace GUI {
	namespace Menu {
		class StartGame;
	}
}

/**
 * Class for the start game dialog.
 */
class GUI::Menu::StartGame: public Menu {
	/** Our client. */
	boost::shared_ptr<Connection::Client> client;

public:
	/**
	 * @copydoc Menu::Menu
	 * @param client The client.
	 */
	StartGame(boost::shared_ptr<Widget> parent, boost::shared_ptr<Connection::Client> client);

	/**
	 * @copydoc Menu::updateState
	 */
	virtual void updateState(sf::RenderWindow& window);
};

#endif

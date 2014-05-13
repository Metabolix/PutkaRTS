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

#ifndef PUTKARTS_GUI_Menu_JoinGame_HPP
#define PUTKARTS_GUI_Menu_JoinGame_HPP

#include "connection/Metaserver.hpp"

#include "gui/menu/Menu.hpp"
#include "gui/widget/List.hpp"
#include "gui/widget/Label.hpp"

#include <SFML/Graphics.hpp>

namespace GUI {
	namespace Menu {
		class JoinGame;
	}
}

/**
 * Class for the start game dialog.
 */
class GUI::Menu::JoinGame: public Menu {
	/** Connection to the metaserver. */
	Connection::Metaserver metaserver;

	/** The game list widget. */
	std::shared_ptr<GUI::Widget::List> gameList;

	/** The selected game id. */
	int selectedId;

	/** A label for the status text. */
	std::shared_ptr<GUI::Widget::Label> statusLabel;

public:
	/**
	 * @copydoc Menu::Menu
	 */
	JoinGame(std::shared_ptr<Widget> parent);

	/**
	 * @copydoc Menu::updateState
	 */
	virtual void updateState(sf::RenderWindow& window);

	/**
	 * Callback for selecting a game from the list.
	 */
	void selectGame(const GUI::Widget::List::Item& item);

	/**
	 * Join the selected game.
	 */
	void joinGame();
};

#endif

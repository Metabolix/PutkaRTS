/*
 * Main menu class definition.
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

#ifndef PUTKARTS_GUI_Menu_MainMenu_HPP
#define PUTKARTS_GUI_Menu_MainMenu_HPP

#include "gui/menu/Menu.hpp"

#include <SFML/Graphics.hpp>

namespace GUI {
	namespace Menu {
		class MainMenu;
	}
}

/**
 * Main class for the game menu.
 */
class GUI::Menu::MainMenu: public Menu {
	/**
	 * Connect to a multiplayer game.
	 *
	 * @param window The window to use for input and rendering.
	 */
	void startMultiGame(sf::RenderWindow& window);

	/**
	 * Start a new game.
	 *
	 * @param window The window to use for input and rendering.
	 */
	void startGame(sf::RenderWindow& window);

	/**
	 * Open the settings menu.
	 *
	 * @param window The window used for input and rendering.
	 */
	void gotoSettings(sf::RenderWindow& window);

public:
	/**
	 * Constructor.
	 *
	 * @param window The window to use for input and rendering.
	 */
	MainMenu(sf::RenderWindow& window);
};

#endif

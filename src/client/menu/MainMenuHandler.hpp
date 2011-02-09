/*
 * Main menu class definition.
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

#ifndef _MAINMENUHANDLER_HPP
#define _MAINMENUHANDLER_HPP

#include <SFML/Graphics.hpp>

/**
 * Main class for the game menu.
 */
class MainMenuHandler {
	/** The view to use for rendering. */
	sf::View view;

	/**
	 * Start a new game.
	 */
	void startGame();
public:
	/**
	 * Run the menu until it quits or a game starts.
	 *
	 * @param window The window to use for input and rendering.
	 */
	void run(sf::RenderWindow& window);
};

#endif

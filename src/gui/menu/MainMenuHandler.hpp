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

#ifndef PUTKARTS_GUI_MainMenuHandler_HPP
#define PUTKARTS_GUI_MainMenuHandler_HPP

#include <SFML/Graphics.hpp>

#include "gui/widget/Container.hpp"
#include "gui/graphics/ImageCache.hpp"

namespace GUI {
	class MainMenuHandler;
}

/**
 * Main class for the game menu.
 */
class GUI::MainMenuHandler: public Widget::Container {
	/** The view to use for rendering. */
	sf::View view;

	/** Main menu images. */
	ImageCache images;

	/** Main menu top logo. */
	sf::Sprite logoSprite;

	/**
	 * Start a new game.
	 *
	 * @param window The window to use for input and rendering.
	 */
	void startGame(sf::RenderWindow& window);
public:
	/**
	 * Constructor.
	 *
	 * @param window The window to use for input and rendering.
	 */
	MainMenuHandler(sf::RenderWindow& window);

	/**
	 * Draw the widget.
	 *
	 * @param window The window to draw to.
	 */
	void draw(sf::RenderWindow& window);
};

#endif

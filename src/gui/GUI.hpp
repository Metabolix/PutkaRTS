/*
 * Global variables for the GUI version of the game.
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

#ifndef PUTKARTS_GUI_HPP
#define PUTKARTS_GUI_HPP

#include <memory>

#include "util/Configuration.hpp"
#include "gui/widget/Widget.hpp"

#include <SFML/Graphics.hpp>

/**
 * Namespace for everything that is specific to the graphical program.
 */
namespace GUI {
	/** Configuration. */
	extern Configuration config;

	/** The current Widget. */
	extern std::shared_ptr<Widget::Widget> currentWidget;

	/** The window for the game */
	extern sf::RenderWindow window;

	/** The current desktop video mode. Hope it doesn't change. */
	extern const sf::VideoMode desktopMode;

	/** The time spent in the previous frame. */
	extern float frameTime;

	/**
	 * Create (or recreate) the main window.
	 */
	void createWindow();
}

#endif

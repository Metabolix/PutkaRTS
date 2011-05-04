/*
 * Settings menu class definition.
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

#ifndef PUTKARTS_GUI_SettingsMenu_HPP
#define PUTKARTS_GUI_SettingsMenu_HPP

#include <SFML/Graphics.hpp>

#include "gui/menu/Menu.hpp"

#include "gui/widget/Widget.hpp"
#include "gui/widget/List.hpp"

namespace GUI {
	namespace Menu {
		class SettingsMenu;
	}
}

/**
 * Class for the setting menu.
 */
class GUI::Menu::SettingsMenu: public Menu {

	/** Index for the selected video mode. */
	int videoMode;

	/** Selected fullscreen state. */
	bool fullscreen;

	/**
	 * Save changes and return to main menu.
	 */
	void applyChanges(sf::RenderWindow& window);

	/**
	 * Set fullscreen state.
	 */
	void setFullScreen(bool state);

	/**
	 * Set selected video mode.
	 */
	void setVideoMode(const GUI::Widget::List::Item& item);

public:
	/**
	 * Constructor.
	 *
	 * @param window The window to use for input and rendering.
	 * @param parent_ Pointer to parent object. Control is returned to parent when menu is closed.
	 */
	SettingsMenu(sf::RenderWindow& window, boost::shared_ptr<Widget> parent_ = boost::shared_ptr<Widget>());
};

#endif

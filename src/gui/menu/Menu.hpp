/*
 * Menu class definition.
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

#ifndef PUTKARTS_GUI_Menu_HPP
#define PUTKARTS_GUI_Menu_HPP

#include <SFML/Graphics.hpp>
#include <boost/shared_ptr.hpp>

#include "gui/graphics/ImageCache.hpp"

#include "gui/widget/Widget.hpp"
#include "gui/widget/Container.hpp"

namespace GUI {
	namespace Menu{
		class Menu;
	}
}

/**
 * Base class for menus.
 */
class GUI::Menu::Menu: public Widget::Container {
	/** The view to use for rendering. */
	sf::View view;

	/** Pointer to parent widget. */
	boost::shared_ptr<Widget> parent;

	/** Menu top logo. */
	sf::Sprite logoSprite;

	/** Keeps track whether the menu is active or not. */
	bool menuIsOpen;

protected:
	/** Menu images. */
	static ImageCache images;

	/**
	 * Set parent and flag the menu as open.
	 *
	 * @param parent_ Pointer to the parent object. Control is returned to parent when menu is closed.
	 */
	void openMenu(boost::shared_ptr<Widget> parent_ = boost::shared_ptr<Widget>());

	/**
	 * Return control to parent if one is specified and flag the menu as closed.
	 */
	void closeMenu();

public:
	/**
	 * Constructor.
	 *
	 * @param parent_ Pointer to the parent object. Control is returned to parent when menu is closed.
	 */
	Menu(boost::shared_ptr<Widget> parent_ = boost::shared_ptr<Widget>());

	/**
	 * Draw the widget.
	 *
	 * @param window The window to draw to.
	 */
	void draw(sf::RenderWindow& window);

	/**
	 * Is the menu open?
	 *
	 * @return Returns true if the menu is flagged as open.
	 */
	bool isOpen() const {
		return menuIsOpen;
	}
};

#endif

/*
 * DropDown widget class definition.
 *
 * Copyright 2011 Jaakko Puntila
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

#ifndef PUTKARTS_GUI_Widget_DropDown_HPP
#define PUTKARTS_GUI_Widget_DropDown_HPP

#include "Widget.hpp"
#include "Button.hpp"
#include "List.hpp"

#include <boost/function.hpp>

namespace GUI {
	namespace Widget {
		class DropDown;
	}
}

/**
 * Class for the drop-down menu widget.
 */
class GUI::Widget::DropDown: public Widget {
public:
	/** Button that opens the drop-down menu */
	GUI::Widget::Button dropButton;
	GUI::Widget::List dropList;

	/** Is the drop-down menu open? */
	bool open;

	/** Callback function type. */
	typedef boost::function<void(const List::Item&)> CallbackType;

private:
	/** Callback function */
	CallbackType action;

protected:
	/**
	 * Opens the menu.
	 */
	virtual void dropButtonPressed();

	/**
	 * Closes the menu and calls action().
	 *
	 * @param item The selected item.
	 */
	virtual void itemSelected(const List::Item& item);

	/** Max height for the drop-down part*/
	float dropDownMaxHeight;

	virtual void calculateDropDownSize();

public:
	/**
	 * Create a drop-down menu.
	 *
	 * @param x X coordinate.
	 * @param y Y coordinate.
	 * @param width Width.
	 * @param height Height.
	 * @param dropDownMaxHeight The maximum height of the menu when opened.
	 * @param callback The action to take when item is selected.
	 */
	DropDown(float x, float y, float width, float height, float dropDownMaxHeight, CallbackType callback = NULL);

	/**
	 * Handle events, namely mouse clicks.
	 *
	 * @param e The event.
	 * @param window The window of the event.
	 * @return True, if this drop-down menu handled the event.
	 */
	virtual bool handleEvent(const sf::Event& e, const sf::RenderWindow& window);

	/**
	 * Draw the drop-down menu.
	 *
	 * @param window The render window.
	 */
	virtual void draw(sf::RenderWindow& window);

	/**
	 * Get the key of the currently selected item key.
	 *
	 * @return Returns the currently selected item. If no item is selected returns dummy item GUI::Widget::List::invalidItem.
	 */
	virtual const GUI::Widget::List::KeyType& getSelectedKey() const;

	/**
	 * Get the key of the currently selected item text.
	 *
	 * @return Returns the currently selected item. If no item is selected returns dummy item GUI::Widget::List::invalidItem.
	 */
	virtual const std::string& getSelectedText() const;

	/**
	 * Insert new item into the menu or replace existing one.
	 *
	 * @param key Key for the item.
	 * @param text Text for the item.
	 */
	virtual void insertItem(const GUI::Widget::List::KeyType& key, const std::string& text);

	/**
	 * Erase item with matching key from the menu.
	 *
	 * @param key Key for the item to erase.
	 */
	virtual void eraseItem(const GUI::Widget::List::KeyType& key);

	/**
	 * Select item with matching key. If no match is found, selection remains unaltered.
	 *
	 * @param key Key for the item to select.
	 */
	virtual void selectItem(const GUI::Widget::List::KeyType& key);
};

#endif

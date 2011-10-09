/*
 * List widget class definition.
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

#ifndef PUTKARTS_GUI_Widget_List_HPP
#define PUTKARTS_GUI_Widget_List_HPP

#include <list>
#include <boost/function.hpp>

#include "Widget.hpp"
#include "Slider.hpp"

namespace GUI {
	namespace Widget {
		class List;
	}
}

/**
 * Class for the list widget.
 */
class GUI::Widget::List: public Widget {
public:
	/** Typedef for Item::key*/
	typedef std::string KeyType;

	/** Struct for the items on the list. */
	struct Item {
		KeyType key;
		std::string text;
	};

	/** Typedef for item container. */
	typedef std::list<Item> ItemContainerType;

	/** Callback function type. */
	typedef boost::function<void(const Item&)> CallbackType;

	/** Invalid item. Used when there is no valid one to use. */
	static Item invalidItem;

private:
	/** Container for the items. */
	ItemContainerType items;

	/** Iterator that points to the selected item or to the end of the container when no item is selected. */
	ItemContainerType::const_iterator selected;

	/** Slider to serve as scrollbar. */
	GUI::Widget::Slider scrollbar;

	/**  Callback function for the list. */
	CallbackType action;

	/** Current scroll position. Lines from the top of the list. */
	int scrollPosition;

protected:
	/** Rendering height of one line. */
	float lineHeight;

	/** Width of list border */
	float borderWidth;

public:
	/**
	 * Create a list.
	 *
	 * @param x X coordinate of the list.
	 * @param y Y coordinate of the list.
	 * @param width Width of the list.
	 * @param height Height of the list.
	 * @param callback The action to take when clicked.
	 */
	List(float x, float y, float width, float height, CallbackType callback = NULL);

	/**
	 * Handle events, namely mouse clicks.
	 *
	 * @param e The event.
	 * @param window The window of the event.
	 * @return True, if this list handled the event.
	 */
	virtual bool handleEvent(const sf::Event& e, const sf::RenderWindow& window);

	/**
	 * Draw the list.
	 *
	 * @param window The render window.
	 */
	virtual void draw(sf::RenderWindow& window);

	/**
	 * Set the scroll position.
	 *
	 * @param pos The new scroll position. (Number of first line to be drawn)
	 */
	virtual void setScrollPosition(int pos);

	/**
	 * Scroll n lines.
	 *
	 * @param n Number of lines to scroll.
	 */
	virtual void scroll(int n);

	/**
	 * Get the key of the currently selected item key.
	 *
	 * @return Returns the currently selected item. If no item is selected returns dummy item GUI::Widget::List::invalidItem.
	 */
	virtual const KeyType& getSelectedKey() const;

	/**
	 * Get the key of the currently selected item text.
	 *
	 * @return Returns the currently selected item. If no item is selected returns dummy item GUI::Widget::List::invalidItem.
	 */
	virtual const std::string& getSelectedText() const;

	/**
	 * Insert new item into the list or replace existing one.
	 *
	 * @param key Key for the item.
	 * @param text Text for the item.
	 */
	virtual void insertItem(const KeyType& key, const std::string& text);

	/**
	 * Erase item with matching key from the list.
	 *
	 * @param key Key for the item to erase.
	 */
	virtual void eraseItem(const KeyType& key);

	/**
	 * Erase all items.
	 */
	virtual void clear();

	/**
	 * Get the borderWidth
	 *
	 * @return Returns the border width.
	 */
	virtual float getBorderWidth() const;

	/**
	 * Get the line height
	 *
	 * @return Returns the line height.
	 */
	virtual float getLineHeight() const;

	/**
	 * Set the line height
	 *
	 * @param lineHeight New line height.
	 */
	virtual void setLineHeight(float lineHeight);

	/**
	 * Get the number of items in the list.
	 *
	 * @return Returns the number of items in the list.
	 */
	virtual unsigned countItems() const;

	/**
	 * Change widget position.
	 */
	virtual void move(float dx, float dy);

	/**
	 * Set widget size.
	 */
	virtual void setSize(float width, float height);

	/**
	 * Select item with matching key. If no match is found, selection remains unaltered.
	 *
	 * @param key Key for the item to select.
	 */
	virtual void selectItem(const GUI::Widget::List::KeyType& key);
};

#endif

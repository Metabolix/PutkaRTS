/*
 * Tab panel widget class definition.
 *
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

#ifndef PUTKARTS_GUI_Widget_TabPanel_HPP
#define PUTKARTS_GUI_Widget_TabPanel_HPP

#include "gui/widget/TabManager.hpp"
#include "gui/widget/Button.hpp"

#include <boost/shared_ptr.hpp>
#include <string>

namespace GUI {
	namespace Widget {
		class TabPanel;
	}
}

/**
 * Class for the tab panel widget.
 */
class GUI::Widget::TabPanel: public GUI::Widget::TabManager {
public:
	/** Struct for buttons. */
	struct Button {
		GUI::Widget::TabManager::TabKeyType key;
		boost::shared_ptr<GUI::Widget::Button> button;
	};

	/** Typedef for button lists. */
	typedef std::list<Button> ButtonListType;

private:
	/** List for buttons. */
	ButtonListType buttons;

	/**
	 * Add new button into the list.
	 *
	 * @param label Text rendered on the button.
	 * @param key Key of the tab this button represents. Used to detect selection.
	 */
	virtual void addButton(const std::string& label, const GUI::Widget::TabManager::TabKeyType& key);

	/**
	 * Remove a button from the list.
	 *
	 * @param key The key to identify the button.
	 */
	virtual void removeButton(const GUI::Widget::TabManager::TabKeyType& key);

	/**
	 * Recalculate button size after adding or removing.
	 */
	virtual void resizeButtons();

public:
	/**
	 * Create a tab panel.
	 *
	 * @param x X coordinate of the tab panel.
	 * @param y Y coordinate of the tab panel.
	 * @param width Width of the tab panel.
	 * @param height Height of the tab panel.
	 */
	TabPanel(float x, float y, float width, float height);

	/**
	 * Handle events.
	 *
	 * @param e The event.
	 * @param window The window of the event.
	 *
	 * @return True, if this checkbox handled the event.
	 */
	virtual bool handleEvent(const sf::Event& e, const sf::RenderWindow& window);

	/**
	 * Draw the tab panel and objects in the selected tab.
	 *
	 * @param window The render window.
	 */
	virtual void draw(sf::RenderWindow& window);

	/**
	 * Update the widget state, if it needs to change without events.
	 *
	 * @param window The current window.
	 */
	virtual void updateState(sf::RenderWindow& window);

	/**
	 * Add a new tab.
	 *
	 * @param key Key for the new tab. If tab with same key already exists, it is replaced by the new one.
	 * @param title Name for the tab.
	 * @param container Container with objects for the tab. If one is not given it will be automatically created when needed.
	 *
	 * @return Returns shared_ptr to the container of the created tab.
	 */
	virtual TabContainerPtrType addTab(const TabKeyType& key, const std::string& title, TabContainerPtrType container = TabContainerPtrType());

	// Allow tabs without buttons using the inherited method.
	using TabManager::addTab;

	/**
	 * Remove a tab.
	 *
	 * @param key Key for the tab to remove.
	 */
	virtual void removeTab(const TabKeyType& key);
};

#endif

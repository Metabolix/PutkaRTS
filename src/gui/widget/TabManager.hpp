/*
 * Tab manager widget class definition.
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

#ifndef PUTKARTS_GUI_Widget_TabManager_HPP
#define PUTKARTS_GUI_Widget_TabManager_HPP

#include <map>
#include <boost/shared_ptr.hpp>

#include "gui/widget/Widget.hpp"
#include "gui/widget/Container.hpp"

namespace GUI {
	namespace Widget {
		class TabManager;
	}
}

/**
 * Class for the tab manager widget.
 */
class GUI::Widget::TabManager: public Widget {
public:
	/** Typedef for tab key. */
	typedef int TabKeyType;

	/** Typedef for tab container. */
	typedef boost::shared_ptr<GUI::Widget::Container> TabContainerPtrType;

	/** Typedef for the tab list*/
	typedef std::map<TabKeyType, TabContainerPtrType> TabListType;

protected:
	/** Container for tabs. */
	TabListType tabs;

	/** Currently selected tab key */
	TabKeyType selectedTabKey;

public:
	/**
	 * Create a tab manager.
	 */
	TabManager();

	/**
	 * Handle events.
	 *
	 * @param e The event.
	 * @param window The window of the event.
	 * @return True, if the event was handled.
	 */
	virtual bool handleEvent(const sf::Event& e, const sf::RenderWindow& window);

	/**
	 * Draw the objects in selected tab.
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
	 * @param container Container with objects for the tab. If one is not given it will be automatically created when needed.
	 *
	 * @return Returns shared_ptr to the container of the created tab.
	 */
	virtual TabContainerPtrType addTab(const TabKeyType& key, TabContainerPtrType container = TabContainerPtrType());

	/**
	 * Get the container of a tab.
	 *
	 * @param key Key of the tab.
	 *
	 * @throw std::runtime_error Thrown if tab with matching key is not found.
	 * @return Returns shared_ptr to the container of the tab with matching key.
	 */
	virtual TabContainerPtrType getTab(const TabKeyType& key);

	/**
	 * Remove a tab.
	 *
	 * @param key Key for the tab to remove.
	 */
	virtual void removeTab(const TabKeyType& key);

	/**
	 * Set the key as the selected key.
	 *
	 * @param key Key for the tab to select.
	 */
	virtual void selectTab(const TabKeyType& key);
};

#endif

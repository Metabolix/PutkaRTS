/*
 * Widget container class definition.
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

#ifndef PUTKARTS_Widget_Container_HPP
#define PUTKARTS_Widget_Container_HPP

#include "Widget.hpp"

#include <list>
#include <boost/shared_ptr.hpp>

namespace Widget {
	/**
	 * Base class for widget containers.
	 */
	class Container: public Widget {
	protected:
		/** The list type to hold object pointers. */
		typedef std::list<boost::shared_ptr<Widget> > WidgetListType;

		/** List of the objects in this container. */
		WidgetListType objects;
	public:
		/**
		 * Insert another object.
		 *
		 * @param object The object to insert.
		 */
		virtual void insert(boost::shared_ptr<Widget> object);

		/**
		 * Erase a previously added object.
		 *
		 * @param object The object to insert.
		 */
		virtual void erase(boost::shared_ptr<Widget> object);

		/**
		 * Draw the objects in this container.
		 */
		virtual void draw(sf::RenderWindow& window);

		/**
		 * Handle an event.
		 *
		 * This version passes the event to each child object until one returns true.
		 *
		 * @param window The window of the event.
		 * @param event The event to handle.
		 * @return true, if one of the children handled the event. Otherwise false.
		 */
		virtual bool handleEvent(const sf::Event& event, const sf::RenderWindow& window);
	};
}

#endif

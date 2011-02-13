/*
 * Base GUI Object class definition.
 *
 * Copyright 2011 Jaakko Puntila
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

#ifndef PUTKARTS_GUI_Object_H
#define PUTKARTS_GUI_Object_H

#include <SFML/Graphics.hpp>

namespace GUI {
	class Object {
	protected:
		/**
		 * Contains the position and dimensions of the object.
		 */
		sf::FloatRect position;

	public:
		virtual ~Object() {
		}

		/**
		 * Handle event.
		 *
		 * @param e The event.
		 * @param window The window of the event.
		 * @return true, if this object handled the event. Otherwise false.
		 */
		virtual bool handleEvent(const sf::Event& e, const sf::RenderWindow& window) {
			return false;
		}

		/**
		 * Draw the GUI object (pure virtual).
		 *
		 * @param window The window to draw to.
		 */
		virtual void draw(sf::RenderWindow& window) = 0;
	};
}

#endif

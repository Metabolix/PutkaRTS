/*
 * GUI Button class definition.
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

#ifndef PUTKARTS_GUI_Button_HPP
#define PUTKARTS_GUI_Button_HPP

#include "Object.hpp"

namespace GUI {
	/**
	 * Class for the button GUI object.
	 */
	class Button: public Object {
		/**
		 * Label for the button.
		 */
		sf::String label;

		/**
		 * Drawable rectangle.
		 */
		sf::Shape rectangle;

	public:
		/**
		 * Create a button with label and position.
		 *
		 * @param text Text is rendered on the button.
		 * @param x X coordinate of the button.
		 * @param y Y coordinate of the button.
		 * @param width Width of the button.
		 * @param height Height of the button.
		 */
		Button(const std::string& text, float x, float y, float width, float height) {
			label = sf::String(text);
			label.SetPosition(x, y);

			position = label.GetRect();

			float scaleX = width / position.GetWidth();
			float scaleY = height / position.GetHeight();

			label.SetScale(scaleX, scaleY);

			position = label.GetRect();

			/* Create the drawable background-rectangle. */
			rectangle = sf::Shape::Rectangle(0, 0, position.GetWidth(), position.GetHeight(), sf::Color(0x55, 0x44, 0x33), 4, sf::Color::Black);
		}

		/**
		 * Handle events, namely mouse clicks.
		 *
		 * @param e The event.
		 * @param window The window of the event.
		 * @return True, if this button handled the event.
		 */
		virtual bool handleEvent(const sf::Event& e, const sf::RenderWindow& window);

		/**
		 * Draw the button.
		 *
		 * @param window The render window.
		 */
		virtual void draw(sf::RenderWindow& window);
	};
}

#endif

/*
 * Button widget class definition.
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

#ifndef PUTKARTS_GUI_Widget_Button_HPP
#define PUTKARTS_GUI_Widget_Button_HPP

#include <boost/function.hpp>

#include "Widget.hpp"
#include "Label.hpp"

namespace GUI {
	namespace Widget {
		class Button;
	}
}

/**
 * Class for the button widget.
 */
class GUI::Widget::Button: public Widget {
public:
	/** Callback function type. */
	typedef boost::function<void()> CallbackType;

private:
	/**
	 * Label for the button.
	 */
	Label label;

	/**
	 * Callback function for the button.
	 */
	CallbackType action;

public:
	/**
	 * Create a button with label and position.
	 *
	 * @param text Text is rendered on the button.
	 * @param x X coordinate of the button.
	 * @param y Y coordinate of the button.
	 * @param width Width of the button.
	 * @param height Height of the button.
	 * @param callback The action to take when clicked.
	 */
	Button(const std::string& text, float x, float y, float width, float height, CallbackType callback);

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
	virtual void draw(sf::RenderWindow& window) {
		draw(window, false);
	};

	/**
	 * Draw the button.
	 *
	 * @param window The render window.
	 * @param highlight If true. Different background color is used.
	 */
	virtual void draw(sf::RenderWindow& window, bool highlight);
};

#endif

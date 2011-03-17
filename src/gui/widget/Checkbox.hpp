/*
 * Checkbox widget class definition.
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

#ifndef PUTKARTS_GUI_Widget_Checkbox_HPP
#define PUTKARTS_GUI_Widget_Checkbox_HPP

#include "Widget.hpp"

#include <boost/function.hpp>

namespace GUI {
	namespace Widget {
		class Checkbox;
	}
}

/**
 * Class for the checkbox widget.
 */
class GUI::Widget::Checkbox: public Widget {
public:
	/** Callback function type. */
	typedef boost::function<void(bool)> CallbackType;

private:
	/**
	 * Label for the checkbox.
	 */
	sf::String label;

	/**
	 * Callback function for the checkbox.
	 */
	CallbackType action;

	/**
	 * Current state of the checkbox.
	 */
	bool isChecked;

public:
	/**
	 * Create a checkbox.
	 *
	 * @param text Text is rendered next to the checkbox.
	 * @param x X coordinate of the checkbox.
	 * @param y Y coordinate of the checkbox.
	 * @param width Width of the checkbox.
	 * @param height Height of the checkbox.
	 * @param callback The action to take when clicked.
	 * @param initialState Initial state of the checkbox.
	 */
	Checkbox(const std::string& text, float x, float y, float width, float height, bool initialState = false, CallbackType callback = NULL);

	/**
	 * Handle events, namely mouse clicks.
	 *
	 * @param e The event.
	 * @param window The window of the event.
	 * @return True, if this checkbox handled the event.
	 */
	virtual bool handleEvent(const sf::Event& e, const sf::RenderWindow& window);

	/**
	 * Draw the checkbox.
	 *
	 * @param window The render window.
	 */
	virtual void draw(sf::RenderWindow& window);

	/**
	 * Set the state of the checkbox.
	 *
	 * @param newState New state for the checkbox.
	 */
	virtual void setState(bool newState);

	/**
	 * Get the state of the checkbox.
	 *
	 * @return Returns the current state of the checkbox.
	 */
	virtual bool getState() const;
};

#endif

/*
 * Slider widget class definition.
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

#ifndef PUTKARTS_GUI_Widget_Slider_HPP
#define PUTKARTS_GUI_Widget_Slider_HPP

#include "Widget.hpp"

#include <boost/function.hpp>

namespace GUI {
	namespace Widget {
		class Slider;
	}
}

/**
 * Class for the slider widget.
 */
class GUI::Widget::Slider: public Widget {
public:
	/** Callback function type. */
	typedef boost::function<void(float)> CallbackType;

private:
	/**
	 * Callback function for the button.
	 */
	CallbackType action;

	float sliderPosition;
	float sliderLength;
	bool vertical;
	bool isDragged;
	float oldMouseCoordinate;

public:
	/**
	 * Create a button with label and position.
	 *
	 * @param x X coordinate of the slider.
	 * @param y Y coordinate of the slider.
	 * @param length Length of the slider
	 * @param thickness Thickness of the slider.
	 * @param height Height of the slider.
	 * @param vertical Orientation of the slider. Horizontal by default.
	 * @param callback The action to take when moved.
	 */
	Slider(float x, float y, float length, float thickness, bool vertical = false, CallbackType callback = NULL);

	/**
	 * Handle events.
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

	/**
	 * Set the slider position.
	 *
	 * @param v New position of the slider. Value between 0.0f - 1.0f.
	 */
	virtual void setScrollPosition(float v);

	/**
	 * Fet the current slider position.
	 *
	 * @return Returns the current slider position. Value between 0.0f - 1.0f.
	 */
	virtual float getScrollPosition() const;

	/**
	 * Set new position the widget.
	 *
	 * @param x New x position
	 * @param y New y position
	 */
	virtual void setPosition(float x, float y);

	/**
	 * Set new size for the widget.
	 *
	 * @param length New length
	 * @param thickness New thickness
	 */
	virtual void setSize(float length, float thickness);
};

#endif

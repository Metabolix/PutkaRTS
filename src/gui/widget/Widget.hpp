/*
 * Base widget class definition.
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

#ifndef PUTKARTS_GUI_Widget_Widget_HPP
#define PUTKARTS_GUI_Widget_Widget_HPP

#include <SFML/Graphics.hpp>

namespace GUI {
	namespace Widget {
		class Widget;
		class Container;
		namespace Color {
			extern const sf::Color background;
			extern const sf::Color backgroundHover;
			extern const sf::Color border;
			extern const sf::Color text;
		}
	}
}

class GUI::Widget::Widget {
protected:
	/**
	 * Contains the position and dimensions of the object.
	 */
	sf::FloatRect position;

	/**
	 * Pointer to the gui container.
	 */
	Container *container;

public:
	/**
	 * Default constructor.
	 */
	Widget():
		container(NULL){
	}

	/**
	 * Virtual destructor to allow polymorphism.
	 */
	virtual ~Widget() {
	}

	/**
	 * Constructor with position parameters.
	 */
	Widget(float x, float y, float width, float height):
		position(x, y, x + width, y + height),
		container(NULL) {

	}

	/**
	 * Constructor with a prefilled position rectangle.
	 */
	Widget(sf::FloatRect position_):
		position(position_),
		container(NULL) {
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
	 * Update the widget state, if it needs to change without events.
	 *
	 * @param window The current window.
	 */
	virtual void updateState(sf::RenderWindow& window) {
		return;
	}

	/**
	 * Draw the widget.
	 *
	 * @param window The window to draw to.
	 */
	virtual void draw(sf::RenderWindow& window) {
		return;
	}

	/**
	 * Set the container pointer.
	 *
	 * @param container_ Pointer to the container.
	 */
	virtual void setContainer(Container *container_) {
		container = container_;
	}

};

#endif

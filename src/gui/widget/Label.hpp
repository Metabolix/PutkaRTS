/*
 * Label widget class definition.
 *
 * Copyright 2011 Mika Katajamäki
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

#ifndef PUTKARTS_GUI_Widget_Label_HPP
#define PUTKARTS_GUI_Widget_Label_HPP

#include "Widget.hpp"

#include <boost/function.hpp>

namespace GUI {
	namespace Widget {
		class Label;
	}
}

/**
 * Class for the Label widget.
 */
class GUI::Widget::Label: public Widget {
	/** Text for the label. */
	sf::String label;

	/** Is the label centered? */
	bool isCentered;

public:
	/**
	 * Create a Label.
	 *
	 * @param text Text to render.
	 * @param x X coordinate of the text.
	 * @param y Y coordinate of the text.
	 * @param height Height of the text.
	 */
	Label(const std::string& text = "", float x = 0, float y = 0, float height = 8);

	/**
	 * Create a Label.
	 *
	 * @param text Text to render.
	 * @param x X coordinate of the text.
	 * @param y Y coordinate of the text.
	 * @param width Maximum width of the text.
	 * @param height Maximum height of the text.
	 */
	Label(const std::string& text, float x, float y, float width, float height);

	/**
	 * Set whether the text is centered or aligned to left.
	 *
	 * @param isCentered_ Is the text centered?
	 */
	virtual void setCentered(bool isCentered_) {
		isCentered = isCentered_;
	}

	/**
	 * Handle events.
	 *
	 * @param e The event.
	 * @param window The window of the event.
	 * @return Always false.
	 */
	virtual bool handleEvent(const sf::Event& e, const sf::RenderWindow& window) {
		return false;
	}

	/**
	 * Draw the Label.
	 *
	 * @param window The render window.
	 */
	virtual void draw(sf::RenderWindow& window);
};

#endif

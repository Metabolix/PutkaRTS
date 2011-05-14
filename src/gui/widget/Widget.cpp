/*
 * Base widget class implementation.
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

#include "Widget.hpp"

namespace GUI {
	namespace Widget {
		namespace Color {
			const sf::Color background(0x55, 0x44, 0x33);
			const sf::Color backgroundHover(0xcc, 0x99, 0x66);
			const sf::Color border(0, 0, 0);
			const sf::Color text(0xff, 0xff, 0xff);
		}
	}
}

void GUI::Widget::Widget::setPosition(float x, float y, float width, float height) {
	move(x - position.Left, y - position.Top);
	setSize(width, height);
}

void GUI::Widget::Widget::move(float dx, float dy) {
	position.Offset(dx, dy);
}

void GUI::Widget::Widget::setSize(float width, float height) {
	position.Right = position.Left + width;
	position.Bottom = position.Top + height;
}

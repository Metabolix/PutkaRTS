/*
 * Label widget class implementation.
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

#include "Label.hpp"


GUI::Widget::Label::Label(const std::string& text, float x, float y, float size):
	Widget(x, y, 1.0f, size),
	label(text) {
	static sf::String maxHeightString("|");
	float scale = position.GetHeight() / maxHeightString.GetRect().GetHeight();
	label.Scale(scale, scale);
}

void GUI::Widget::Label::draw(sf::RenderWindow& window) {
	label.SetPosition(position.Right, position.Top);

	window.Draw(label);
}

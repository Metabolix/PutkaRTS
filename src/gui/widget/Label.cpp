/*
 * Label widget class implementation.
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

#include <limits>
#include <algorithm>

#include "Label.hpp"

GUI::Widget::Label::Label(const std::string& text, float x, float y, float height):
	Widget(x, y, std::numeric_limits<float>::infinity(), height),
	label(stringFromUtf8(text), font),
	isCentered(false) {
	label.setColor(Color::text);
}

GUI::Widget::Label::Label(const std::string& text, float x, float y, float width, float height):
	Widget(x, y, width, height),
	label(stringFromUtf8(text), font),
	isCentered(false) {
	label.setColor(Color::text);
}

void GUI::Widget::Label::setText(const std::string& text) {
	label = sf::Text(stringFromUtf8(text), font);
}

void GUI::Widget::Label::draw(sf::RenderWindow& window) {
	const sf::String& data = label.getString();
	if (data.isEmpty()) {
		return;
	}
	size_t lineCount = std::count(data.begin(), data.end(), '\n') + (*(data.end() - 1) == '\n' ? 0 : 1);

	static float lineHeightForSize1;
	if (!lineHeightForSize1) {
		sf::Text maxHeightString("|gY", font);
		lineHeightForSize1 = 1.25 * maxHeightString.getLocalBounds().height / maxHeightString.getCharacterSize();
	}

	label.setColor(Color::text);
	label.setScale(1, 1);

	// Calculate height; using label.getRect().height makes different strings have different sizes.
	float lineHeight = label.getCharacterSize() * lineHeightForSize1;
	float height = lineHeight * lineCount;
	float scaleY = position.height / height;
	float scaleX = position.width / label.getLocalBounds().width;
	float scale = std::min(scaleX, scaleY);

	if (isCentered) {
		label.setOrigin(label.getLocalBounds().width / 2, 0);
		label.setPosition(position.left + position.width / 2, position.top);
	} else {
		label.setOrigin(0, 0);
		label.setPosition(position.left, position.top);
	}
	label.setScale(scale, scale);
	window.draw(label);
}

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
	label(text),
	isCentered(false) {
	label.SetColor(Color::text);
}

GUI::Widget::Label::Label(const std::string& text, float x, float y, float width, float height):
	Widget(x, y, width, height),
	label(text),
	isCentered(false) {
	label.SetColor(Color::text);
}

void GUI::Widget::Label::draw(sf::RenderWindow& window) {
	const sf::Unicode::UTF32String& data = label.GetText();
	if (data.empty()) {
		return;
	}
	size_t lineCount = std::count(data.begin(), data.end(), '\n') + (*data.rbegin() == '\n' ? 0 : 1);

	static float lineHeightForSize1;
	if (!lineHeightForSize1) {
		sf::String maxHeightString("|");
		lineHeightForSize1 = 1.05 * maxHeightString.GetRect().GetHeight() / maxHeightString.GetSize();
	}

	label.SetColor(Color::text);
	label.SetScale(1, 1);

	// Calculate height; using label.GetRect().GetHeight() makes different strings have different sizes.
	float lineHeight = label.GetSize() * lineHeightForSize1;
	float height = lineHeight * lineCount;
	float scaleY = position.GetHeight() / height;
	float scaleX = position.GetWidth() / label.GetRect().GetWidth();
	float scale = std::min(scaleX, scaleY);

	if (isCentered) {
		label.SetCenter(label.GetRect().GetWidth() / 2, 0);
		label.SetPosition(position.Left + position.GetWidth() / 2, position.Top);
	} else {
		label.SetCenter(0, 0);
		label.SetPosition(position.Left, position.Top);
	}
	label.SetScale(scale, scale);
	window.Draw(label);
}

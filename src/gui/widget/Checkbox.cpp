/*
 * Checkbox widget class implementation.
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

#include "Checkbox.hpp"

#include <algorithm>

GUI::Widget::Checkbox::Checkbox(const std::string& text, float x, float y, float width, float height, bool initialState, CallbackType callback):
	Widget(x, y, width, height),
	label(text),
	action(callback),
	isChecked(initialState) {
	static sf::String maxHeightString("|");
	label.SetCenter(0.0f, maxHeightString.GetRect().GetHeight() / 2);
}

bool GUI::Widget::Checkbox::handleEvent(const sf::Event& e, const sf::RenderWindow& window) {
	if (e.Type != sf::Event::MouseButtonPressed || e.MouseButton.Button != sf::Mouse::Left) {
		return false;
	}

	sf::Vector2f mouse(window.ConvertCoords(e.MouseButton.X, e.MouseButton.Y));

	if (position.Contains(mouse.x, mouse.y)) {
		isChecked = !isChecked;
		if (action) {
			action(isChecked);
		}
		return true;
	}

	return false;
}

void GUI::Widget::Checkbox::draw(sf::RenderWindow& window) {
	const int bw = (std::min(position.GetWidth(), position.GetHeight()) < 40 ? 2 : 4); // Border width

	label.SetPosition(position.Right + 6.0f, (position.Top + position.Bottom) / 2.0f);

	float scale = (position.GetHeight() - 2 * bw) / label.GetRect().GetHeight() * 1.25f;
	label.Scale(scale, scale);

	window.Draw(sf::Shape::Rectangle(position.Left + bw, position.Top + bw, position.Right - bw, position.Bottom - bw, Color::background, bw, Color::border));
	window.Draw(label);

	if (isChecked) {
		window.Draw(sf::Shape::Line(position.Left + 2 * bw, position.Top + 2 * bw, position.Right - 2 * bw, position.Bottom - 2 * bw, bw, Color::text));
		window.Draw(sf::Shape::Line(position.Right - 2 * bw, position.Top + 2 * bw, position.Left + 2 * bw, position.Bottom - 2 * bw, bw, Color::text));
	}
}

void GUI::Widget::Checkbox::setState(bool newState) {
	isChecked = newState;
}

bool GUI::Widget::Checkbox::getState() const {
	return isChecked;
}

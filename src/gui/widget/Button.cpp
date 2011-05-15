/*
 * Button widget class implementation.
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

#include "Button.hpp"

#include <algorithm>

GUI::Widget::Button::Button(const std::string& text, float x, float y, float width, float height, CallbackType callback):
	Widget(x, y, width, height),
	label(text),
	action(callback) {
	label.setCentered(true);
}

bool GUI::Widget::Button::handleEvent(const sf::Event& e, const sf::RenderWindow& window) {
	if (e.Type != sf::Event::MouseButtonPressed || e.MouseButton.Button != sf::Mouse::Left) {
		return false;
	}

	sf::Vector2f mouse(window.ConvertCoords(e.MouseButton.X, e.MouseButton.Y));

	if (position.Contains(mouse.x, mouse.y)) {
		action();
		return true;
	}

	return false;
}

void GUI::Widget::Button::draw(sf::RenderWindow& window, bool highlight) {
	const int bw = (std::min(position.GetWidth(), position.GetHeight()) < 40 ? 2 : 4); // Border width
	const sf::Input& input(window.GetInput());
	sf::Vector2f mouse(window.ConvertCoords(input.GetMouseX(), input.GetMouseY()));

	sf::Color background(Color::background);

	if (position.Contains(mouse.x, mouse.y) || highlight) {
		background = Color::backgroundHover;
	}

	window.Draw(sf::Shape::Rectangle(position.Left + bw, position.Top + bw, position.Right - bw, position.Bottom - bw, background, bw, Color::border));

	label.setPosition(position.Left + 2 * bw, position.Top + 2 * bw, position.GetWidth() - 4 * bw, position.GetHeight() - 4 * bw);
	label.draw(window);
}

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

#include <algorithm>

#include "Button.hpp"

GUI::Widget::Button::Button(const std::string& text, float x, float y, float width, float height, CallbackType callback):
	Widget(x, y, width, height),
	label(text),
	action(callback) {
	label.setCentered(true);
}

bool GUI::Widget::Button::handleEvent(const sf::Event& e, const sf::RenderWindow& window) {
	if (e.type != sf::Event::MouseButtonPressed || e.mouseButton.button != sf::Mouse::Left) {
		return false;
	}

	sf::Vector2f mouse(window.mapPixelToCoords(sf::Vector2i(e.mouseButton.x, e.mouseButton.y)));

	if (position.contains(mouse)) {
		action();
		return true;
	}

	return false;
}

void GUI::Widget::Button::draw(sf::RenderWindow& window, bool highlight) {
	const int bw = (std::min(position.width, position.height) < 40 ? 2 : 4); // Border width
	sf::Vector2f mouse(window.mapPixelToCoords(sf::Mouse::getPosition(window)));

	sf::RectangleShape tmp;
	tmp.setSize(sf::Vector2f(position.width - 2 * bw, position.height - 2 * bw));
	tmp.setPosition(position.left + bw, position.top + bw);
	tmp.setFillColor((highlight || position.contains(mouse)) ? Color::backgroundHover : Color::background);
	tmp.setOutlineColor(Color::border);
	tmp.setOutlineThickness(bw);
	window.draw(tmp);

	label.setPosition(position.left + 2 * bw, position.top + 2 * bw, position.width - 4 * bw, position.height - 4 * bw);
	label.draw(window);
}

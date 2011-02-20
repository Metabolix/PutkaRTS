/*
 * GUI Button class implementation.
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

bool GUI::Button::handleEvent(const sf::Event& e, const sf::RenderWindow& window) {
	if (e.Type != sf::Event::MouseButtonPressed || e.MouseButton.Button != sf::Mouse::Left) {
		return false;
	}

	sf::Vector2f mouse(window.ConvertCoords(e.MouseButton.X, e.MouseButton.Y));

	if (position.Contains(mouse.x, mouse.y)) {
		// TODO: Handle click.
		return true;
	}

	return false;
}

void GUI::Button::draw(sf::RenderWindow& window) {
	const sf::Input& input(window.GetInput());
	sf::Vector2f mouse(window.ConvertCoords(input.GetMouseX(), input.GetMouseY()));

	label.SetPosition(position.Left, position.Top);
	rectangle.SetPosition(position.Left, position.Top);
	rectangle.SetColor(sf::Color(0x55, 0x44, 0x33));

	if (position.Contains(mouse.x, mouse.y)) {
		rectangle.SetColor(sf::Color(0xaa, 0x77, 0x44));
	}

	window.Draw(rectangle);
	window.Draw(label);
}

/*
 * Slider widget class implementation.
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

#include "Slider.hpp"

#include <algorithm>

GUI::Widget::Slider::Slider(float x, float y, float length, float thickness, bool _vertical, CallbackType callback, float initialValue):
	Widget(x, y, (_vertical ? thickness : length), (_vertical ? length : thickness)),
	action(callback),
	vertical(_vertical) {
	sliderPosition = 0;
	sliderLength = length / 5;
	isDragged = false;
	setScrollPosition(initialValue);
}

bool GUI::Widget::Slider::handleEvent(const sf::Event& e, const sf::RenderWindow& window) {
	if (e.Type == sf::Event::MouseButtonPressed && e.MouseButton.Button == sf::Mouse::Left) {
		sf::Vector2f mouse(window.ConvertCoords(e.MouseButton.X, e.MouseButton.Y));

		if (position.Contains(mouse.x, mouse.y)) {

			float mousePos = (vertical?mouse.y - position.Top:mouse.x - position.Left);

			if (mousePos >= sliderPosition && mousePos < sliderPosition + sliderLength) {
				//slider pressed
				isDragged = true;
				oldMouseCoordinate = (vertical ? mouse.y : mouse.x);
			}

			return true;
		}
	} else if (e.Type == sf::Event::MouseButtonReleased && e.MouseButton.Button == sf::Mouse::Left) {
		isDragged = false;
		return false;
	} else if (isDragged && e.Type == sf::Event::MouseMoved) {
		sf::Vector2f mouse(window.ConvertCoords(e.MouseMove.X, e.MouseMove.Y));

		sliderPosition += (vertical ? mouse.y : mouse.x) - oldMouseCoordinate;
		sliderPosition = std::max(0.0f, std::min(sliderPosition, (vertical ? position.GetHeight() : position.GetWidth()) - sliderLength));

		oldMouseCoordinate = (vertical ? mouse.y : mouse.x);

		if (action) {
			action(getScrollPosition());
		}

		return true;
	}
	return false;
}

void GUI::Widget::Slider::draw(sf::RenderWindow& window) {
	const int bw = 2; // Border width
	const sf::Input& input(window.GetInput());
	sf::Vector2f mouse(window.ConvertCoords(input.GetMouseX(), input.GetMouseY()));

	//bg
	window.Draw(sf::Shape::Rectangle(position.Left, position.Top, position.Right, position.Bottom, Color::background));

	if (vertical) {
		window.Draw(
			sf::Shape::Rectangle(
				position.Left + bw,
				position.Top + sliderPosition + bw,
				position.Right - bw,
				position.Top + sliderPosition + sliderLength - bw,
				Color::backgroundHover,
				bw,
				Color::border
			)
		);
	} else {
		window.Draw(
			sf::Shape::Rectangle(
				position.Left + sliderPosition + bw,
				position.Top + bw,
				position.Left + sliderPosition + sliderLength - bw,
				position.Bottom - bw,
				Color::backgroundHover,
				bw,
				Color::border
			)
		);
	}
}

void GUI::Widget::Slider::setScrollPosition(float v) {
	v = std::max(0.0f, std::min(v, 1.0f));
	sliderPosition = v * ((vertical ? position.GetHeight() : position.GetWidth()) - sliderLength);
}

float GUI::Widget::Slider::getScrollPosition() const {
	return sliderPosition / ((vertical ? position.GetHeight() : position.GetWidth()) - sliderLength);
}

void GUI::Widget::Slider::setPosition(float x, float y) {
	position.Offset(x - position.Left, y - position.Top);
}

void GUI::Widget::Slider::setSize(float length, float thickness) {
	position.Right = position.Left + (vertical ? thickness : length);
	position.Bottom = position.Top + (vertical ? length : thickness);
	sliderLength = length / 5;
}

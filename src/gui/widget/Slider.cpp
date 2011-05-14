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

GUI::Widget::Slider::Slider(float x, float y, float length, float thickness, bool _vertical, CallbackType callback, float rangeMin_, float rangeMax_, float initialValue):
	Widget(x, y, (_vertical ? thickness : length), (_vertical ? length : thickness)),
	action(callback),
	vertical(_vertical) {
	sliderPosition = 0;
	isDragged = false;
	rangeMin = rangeMin_;
	rangeMax = rangeMax_;
	setScrollPosition(initialValue);
}

bool GUI::Widget::Slider::handleEvent(const sf::Event& e, const sf::RenderWindow& window) {
	if (e.Type == sf::Event::MouseButtonPressed && e.MouseButton.Button == sf::Mouse::Left) {
		sf::Vector2f mouse(window.ConvertCoords(e.MouseButton.X, e.MouseButton.Y));

		if (position.Contains(mouse.x, mouse.y)) {

			float mousePos = (vertical?mouse.y - position.Top:mouse.x - position.Left);

			if (mousePos >= sliderPosition && mousePos < sliderPosition + getSliderLength()) {
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
		sliderPosition = std::max(0.0f, std::min(sliderPosition, (vertical ? position.GetHeight() : position.GetWidth()) - getSliderLength()));

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
				position.Top + sliderPosition + getSliderLength() - bw,
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
				position.Left + sliderPosition + getSliderLength() - bw,
				position.Bottom - bw,
				Color::backgroundHover,
				bw,
				Color::border
			)
		);
	}
}

void GUI::Widget::Slider::setScrollPosition(float v) {
	v = inverseTransformValue(v, rangeMin, rangeMax);
	sliderPosition = v * ((vertical ? position.GetHeight() : position.GetWidth()) - getSliderLength());
}

float GUI::Widget::Slider::getScrollPosition() const {
	return transformValue(sliderPosition / ((vertical ? position.GetHeight() : position.GetWidth()) - getSliderLength()), rangeMin, rangeMax);
}

float GUI::Widget::Slider::getSliderLength() const {
	return 0.2 * (vertical ? position.GetHeight() : position.GetWidth());
}

float GUI::Widget::Slider::transformValue(float raw, float min, float max) {
	return min + std::max(0.0f, std::min(raw, 1.0f)) * (max - min);
}

float GUI::Widget::Slider::inverseTransformValue(float value, float min, float max) {
	return std::max(0.0f, std::min((value - min) / (max - min), 1.0f));
}

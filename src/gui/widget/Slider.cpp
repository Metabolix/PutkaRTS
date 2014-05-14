#include <algorithm>

#include "Slider.hpp"

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
	if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
		sf::Vector2f mouse(window.mapPixelToCoords(sf::Vector2i(e.mouseButton.x, e.mouseButton.y)));

		if (position.contains(mouse)) {

			float mousePos = (vertical ? mouse.y - position.top : mouse.x - position.left);

			if (mousePos >= sliderPosition && mousePos < sliderPosition + getSliderLength()) {
				//slider pressed
				isDragged = true;
				oldMouseCoordinate = (vertical ? mouse.y : mouse.x);
			}

			return true;
		}
	} else if (e.type == sf::Event::MouseButtonReleased && e.mouseButton.button == sf::Mouse::Left) {
		isDragged = false;
		return false;
	} else if (isDragged && e.type == sf::Event::MouseMoved) {
		sf::Vector2f mouse(window.mapPixelToCoords(sf::Vector2i(e.mouseMove.x, e.mouseMove.y)));

		sliderPosition += (vertical ? mouse.y : mouse.x) - oldMouseCoordinate;
		sliderPosition = std::max(0.0f, std::min(sliderPosition, (vertical ? position.height : position.width) - getSliderLength()));

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
	sf::Vector2f mouse(window.mapPixelToCoords(sf::Mouse::getPosition(window)));

	sf::RectangleShape bg;
	bg.setSize(sf::Vector2f(position.width, position.height));
	bg.setPosition(position.left, position.top);
	bg.setFillColor(Color::background);
	window.draw(bg);

	sf::RectangleShape handle;
	handle.setFillColor(Color::backgroundHover);
	handle.setOutlineColor(Color::border);
	handle.setOutlineThickness(bw);
	if (vertical) {
		handle.setSize(sf::Vector2f(position.width - 2 * bw, getSliderLength() - bw));
		handle.setPosition(position.left + bw, position.top + sliderPosition + bw);
	} else {
		handle.setSize(sf::Vector2f(getSliderLength() - bw, position.height - 2 * bw));
		handle.setPosition(position.left + sliderPosition + bw, position.top + bw);
	}
	window.draw(handle);
}

void GUI::Widget::Slider::setScrollPosition(float v) {
	v = inverseTransformValue(v, rangeMin, rangeMax);
	sliderPosition = v * ((vertical ? position.height : position.width) - getSliderLength());
}

float GUI::Widget::Slider::getScrollPosition() const {
	return transformValue(sliderPosition / ((vertical ? position.height : position.width) - getSliderLength()), rangeMin, rangeMax);
}

float GUI::Widget::Slider::getSliderLength() const {
	return 0.2 * (vertical ? position.height : position.width);
}

float GUI::Widget::Slider::transformValue(float raw, float min, float max) {
	return min + std::max(0.0f, std::min(raw, 1.0f)) * (max - min);
}

float GUI::Widget::Slider::inverseTransformValue(float value, float min, float max) {
	return std::max(0.0f, std::min((value - min) / (max - min), 1.0f));
}

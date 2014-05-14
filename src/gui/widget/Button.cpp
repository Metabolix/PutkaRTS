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

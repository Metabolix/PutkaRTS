#include "MouseTracker.hpp"

void GUI::Game::MouseTracker::update(const sf::RenderWindow& window, const sf::View& view, unsigned int x_, unsigned int y_) {
	x = x_;
	y = y_;
	update(window, view);
}

void GUI::Game::MouseTracker::update(const sf::RenderWindow& window, const sf::View& view) {
	sf::Vector2f tmp = window.mapPixelToCoords(sf::Vector2i(x, y), view);
	position.x = tmp.x;
	position.y = tmp.y;
}

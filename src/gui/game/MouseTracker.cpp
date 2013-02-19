/*
 * Mouse tracker class implementation.
 *
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

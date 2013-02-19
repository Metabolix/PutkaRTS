/*
 * Scrolling view class implementation.
 *
 * Copyright 2011 Lauri Kenttä
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

#include <cmath>

#include "ScrollingView.hpp"

#include "gui/GUI.hpp"

GUI::Game::ScrollingView::ScrollingView(sf::RenderWindow& window, sf::Vector2f contentSize_, std::size_t pixelsPerUnit_):
	contentSize(contentSize_),
	pixelsPerUnit(pixelsPerUnit_),
	mouseDragging(false) {
	reset(window);
}

void GUI::Game::ScrollingView::reset(sf::RenderWindow& window) {
	sf::Vector2f location = getCenter();
	static_cast<sf::View&>(*this) = window.getDefaultView();
	setCenter(location);
	zoom(1.0f / pixelsPerUnit);
}

void GUI::Game::ScrollingView::update(sf::RenderWindow& window) {
	float time = GUI::frameTime;
	const sf::Vector2i mouse = sf::Mouse::getPosition(window);

	const float zoomSpeed = GUI::config.getDouble("gameUI.zoomSpeed", 2);

	// zoom with pgUp and pgDown
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageDown)) {
		zoom(std::pow(2, zoomSpeed * -time));
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageUp)) {
		zoom(std::pow(2, zoomSpeed * time));
	}

	// limit zoom level to at least 4 tiles or at most map size x2
	sf::Vector2f size = getSize();
	if (size.x < 4) {
		size *= 4 / size.x;
	}
	if (size.y < 4) {
		size *= 4 / size.y;
	}
	if (size.x / 2 > contentSize.x) {
		size *= 2 * contentSize.x / size.x;
	}
	if (size.y / 2 > contentSize.y) {
		size *= 2 * contentSize.y / size.y;
	}
	setSize(size);

	const float keyboardScrollSpeed = size.x * GUI::config.getDouble("gameUI.keyboardScrollSpeed", 1);
	const float borderScrollSpeed = size.x * GUI::config.getDouble("gameUI.borderScrollSpeed", 1);
	const int borderScrollThreshold = GUI::config.getInt("gameUI.borderScrollThreshold", 5);
	const bool reverseDrag = GUI::config.getBool("gameUI.reverseDrag", false);
	const bool grabCursorOnDrag = GUI::config.getBool("gameUI.grabCursorOnDrag", true);

	// scroll map with arrow keys
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		move(keyboardScrollSpeed * time, 0);
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		move(-keyboardScrollSpeed * time, 0);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		move(0, keyboardScrollSpeed * time);
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		move(0, -keyboardScrollSpeed * time);
	}

	// border scrolling
	if (!mouseDragging) {
		if (mouse.x < borderScrollThreshold) {
			move(-borderScrollSpeed * time, 0);
		} else if (mouse.x > (int) window.getSize().x - borderScrollThreshold) {
			move(borderScrollSpeed * time, 0);
		}
		if (mouse.y < borderScrollThreshold) {
			move(0, -borderScrollSpeed * time);
		} else if (mouse.y > (int) window.getSize().y - borderScrollThreshold) {
			move(0, borderScrollSpeed * time);
		}
	}

	// drag with right mouse
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		mouseDragging = false;
	} else {
		if (mouseDragging) {
			sf::Vector2f mapDragOrigin = window.mapPixelToCoords(dragOrigin, *this);
			sf::Vector2f mapDragDestination = window.mapPixelToCoords(mouse, *this);
			sf::Vector2f change(mapDragOrigin - mapDragDestination);
			if (reverseDrag) {
				change = -change;
			}
			move(change);
		}
		if (mouseDragging && grabCursorOnDrag) {
			sf::Mouse::setPosition(dragOrigin, window);
		} else {
			dragOrigin = mouse;
		}
		mouseDragging = true;
	}

	//reset view with home key.
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Home)) {
		reset(window);
	}

	// boundaries
	sf::Vector2f viewCenter = getCenter();
	viewCenter.x = std::max<float>(0.0f, std::min<float>(viewCenter.x, contentSize.x));
	viewCenter.y = std::max<float>(0.0f, std::min<float>(viewCenter.y, contentSize.y));
	setCenter(viewCenter);
}

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

#include "ScrollingView.hpp"

#include "gui/GUI.hpp"

GUI::Game::ScrollingView::ScrollingView(sf::RenderWindow& window, sf::Vector2f contentSize_, std::size_t pixelsPerUnit_):
	contentSize(contentSize_),
	pixelsPerUnit(pixelsPerUnit_),
	mouseDragging(false) {
	reset(window);
}

void GUI::Game::ScrollingView::reset(sf::RenderWindow& window) {
	sf::Vector2f location = GetCenter();
	static_cast<sf::View&>(*this) = window.GetDefaultView();
	SetCenter(location);
	Zoom(pixelsPerUnit);
}

void GUI::Game::ScrollingView::update(sf::RenderWindow& window) {
	float time = window.GetFrameTime();
	const sf::Input & input = window.GetInput();

	const float zoomSpeed = GUI::config.getDouble("gameUI.zoomSpeed", 2);

	// zoom with pgUp and pgDown
	if (input.IsKeyDown(sf::Key::PageDown)) {
		Zoom(pow(2, zoomSpeed * time));
	} else if (input.IsKeyDown(sf::Key::PageUp)) {
		Zoom(pow(2, zoomSpeed * -time));
	}

	// limit zoom level to at least 4 tiles or at most map size x2
	sf::Vector2f halfSize = GetHalfSize();
	if (halfSize.x < 2) {
		halfSize *= 2 / halfSize.x;
	}
	if (halfSize.y < 2) {
		halfSize *= 2 / halfSize.y;
	}
	if (halfSize.x > contentSize.x) {
		halfSize *= contentSize.x / halfSize.x;
	}
	if (halfSize.y > contentSize.y) {
		halfSize *= contentSize.y / halfSize.y;
	}
	SetHalfSize(halfSize);

	const float keyboardScrollSpeed = 2 * halfSize.x * GUI::config.getDouble("gameUI.keyboardScrollSpeed", 1);
	const float borderScrollSpeed = 2 * halfSize.x * GUI::config.getDouble("gameUI.borderScrollSpeed", 1);
	const int borderScrollThreshold = GUI::config.getInt("gameUI.borderScrollThreshold", 5);
	const bool reverseDrag = GUI::config.getBool("gameUI.reverseDrag", false);
	const bool grabCursorOnDrag = GUI::config.getBool("gameUI.grabCursorOnDrag", true);

	// scroll map with arrow keys
	if (input.IsKeyDown(sf::Key::Right)) {
		Move(keyboardScrollSpeed * time, 0);
	} else if (input.IsKeyDown(sf::Key::Left)) {
		Move(-keyboardScrollSpeed * time, 0);
	}
	if (input.IsKeyDown(sf::Key::Down)) {
		Move(0, keyboardScrollSpeed * time);
	} else if (input.IsKeyDown(sf::Key::Up)) {
		Move(0, -keyboardScrollSpeed * time);
	}

	// border scrolling
	if (!mouseDragging) {
		if (input.GetMouseX() < borderScrollThreshold) {
			Move(-borderScrollSpeed * time, 0);
		} else if (input.GetMouseX() > (int) window.GetWidth() - borderScrollThreshold) {
			Move(borderScrollSpeed * time, 0);
		}
		if (input.GetMouseY() < borderScrollThreshold) {
			Move(0, -borderScrollSpeed * time);
		} else if (input.GetMouseY() > (int) window.GetHeight() - borderScrollThreshold) {
			Move(0, borderScrollSpeed * time);
		}
	}

	// drag with right mouse
	if (!input.IsMouseButtonDown(sf::Mouse::Right)) {
		mouseDragging = false;
	} else {
		if (mouseDragging) {
			sf::Vector2f mapDragOrigin = window.ConvertCoords(dragOrigin.x, dragOrigin.y, this);
			sf::Vector2f mapDragDestination = window.ConvertCoords(input.GetMouseX(), input.GetMouseY(), this);
			sf::Vector2f change(mapDragOrigin - mapDragDestination);
			if (reverseDrag) {
				change = -change;
			}
			Move(change);
		}
		if (mouseDragging && grabCursorOnDrag) {
			window.SetCursorPosition(dragOrigin.x, dragOrigin.y);
		} else {
			dragOrigin = sf::Vector2f(input.GetMouseX(), input.GetMouseY());
		}
		mouseDragging = true;
	}

	//reset view with home key.
	if (input.IsKeyDown(sf::Key::Home)) {
		reset(window);
	}

	// boundaries
	sf::Vector2f viewCenter = GetCenter();
	viewCenter.x = std::max<float>(0.0f, std::min<float>(viewCenter.x, contentSize.x));
	viewCenter.y = std::max<float>(0.0f, std::min<float>(viewCenter.y, contentSize.y));
	SetCenter(viewCenter);
}

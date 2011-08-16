/*
 * Mouse tracker class definition.
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

#ifndef PUTKARTS_GUI_MouseTracker_HPP
#define PUTKARTS_GUI_MouseTracker_HPP

#include "util/Vector2.hpp"

#include <SFML/Graphics.hpp>

namespace GUI {
	class MouseTracker;
}

/**
 * Class for keeping track of mouse state.
 */
class GUI::MouseTracker {
	/** Screen coordinates. */
	unsigned int x, y;

	/** Game coordinates. */
	Vector2<SIUnit::Position> position;

public:
	/**
	 * Update the mouse state.
	 *
	 * @param window The window.
	 * @param view The game view.
	 * @param x The x coordinate.
	 * @param y The y coordinate.
	 */
	void update(const sf::RenderWindow& window, const sf::View& view, unsigned int x, unsigned int y);

	/**
	 * Update the mouse state using the stored coordinates.
	 *
	 * @param window The window.
	 * @param view The game view.
	 */
	void update(const sf::RenderWindow& window, const sf::View& view);

	/**
	 * Get the position in game coordinates.
	 */
	const Vector2<SIUnit::Position>& getPosition() const {
		return position;
	}
};

#endif

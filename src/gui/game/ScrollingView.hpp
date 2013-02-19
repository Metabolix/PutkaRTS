/*
 * Scrolling view class definition.
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

#ifndef PUTKARTS_GUI_Game_ScrollingView_HPP
#define PUTKARTS_GUI_Game_ScrollingView_HPP

#include <SFML/Graphics.hpp>

namespace GUI {
	namespace Game {
		class ScrollingView;
	}
}

/**
 * Class for scrolling.
 */
class GUI::Game::ScrollingView: public sf::View {
	/** Size of the scrollable area. */
	const sf::Vector2f contentSize;

	/** The default zoom level. */
	const std::size_t pixelsPerUnit;

	/** Is map being dragged with mouse? */
	bool mouseDragging;

	/** Window coordinates for drag origin */
	sf::Vector2i dragOrigin;

public:
	/**
	 * Constructor.
	 *
	 * @param window The window to use for view data.
	 * @param contentSize Size of the scrollable area.
	 * @param pixelsPerUnit How big is one unit in pixels?
	 */
	ScrollingView(sf::RenderWindow& window, sf::Vector2f contentSize, std::size_t pixelsPerUnit);

	/**
	 * Reset view used for map rendering
	 *
	 * @param window The window to use for view data.
	 */
	void reset(sf::RenderWindow& window);

	/**
	 * Handle map scrolling calculations.
	 *
	 * @param window The window to use for input and rendering.
	 */
	void update(sf::RenderWindow& window);
};

#endif

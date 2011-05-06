/*
 * GUI game handler class definition.
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

#ifndef PUTKARTS_GUI_GameHandler_HPP
#define PUTKARTS_GUI_GameHandler_HPP

#include "game/GameConnection.hpp"

#include <SFML/Graphics.hpp>
#include <boost/shared_ptr.hpp>

#include "gui/widget/Container.hpp"
#include "gui/graphics/ImageCache.hpp"
#include "gui/menu/SettingsMenu.hpp"

namespace GUI {
	class GameHandler;
}

/**
 * Class for the game GUI.
 */
class GUI::GameHandler: public Widget::Container {
	/** The view used while rendering the GUI. */
	sf::View guiView;

	/** All images needed for rendering. */
	ImageCache images;

	/** The game connection to use. */
	boost::shared_ptr<GameConnection> connection;

	/** Camera position and zoom */
	sf::View gameView;

	/** Size of one map tile */
	static const int tileSize;

	/** Is map being dragged with mouse?*/
	bool mouseDrag;

	/** Window coordinates for drag origin*/
	sf::Vector2f dragOrigin;

	/** Used to break the main loop from within member functions. */
	bool gameClosed;

	/** The settings menu, if opened. */
	boost::shared_ptr<Menu::SettingsMenu> settingsMenu;
public:
	/**
	 * Constructor.
	 *
	 * @param connection The game connection to use.
	 * @param window The window to use for input and rendering.
	 */
	GameHandler(boost::shared_ptr<GameConnection> connection, sf::RenderWindow& window);

	/**
	 * Load map graphics etc.
	 */
	void loadMapData();

	/**
	 * Draw the map and the units.
	 *
	 * @param window The window to use for input and rendering.
	 */
	void drawGame(sf::RenderWindow& window) const;

	/**
	 * Exit the game.
	 */
	void exit();

	/**
	 * Handle event.
	 *
	 * @param e The event.
	 * @param window The window of the event.
	 * @return true, if this object handled the event. Otherwise false.
	 */
	bool handleEvent(const sf::Event& e, const sf::RenderWindow& window);

	/**
	 * Draw the widget.
	 *
	 * @param window The window to draw to.
	 */
	void draw(sf::RenderWindow& window);

	/**
	 * Handle map scrolling calculations.
	 *
	 * @param window The window to use for input and rendering.
	 */
	void handleScrolling(sf::RenderWindow& window);

	/**
	 * Reset view used for map rendering
	 *
	 * @param window The window to use for view data.
	 * @param resetLocation Reset location as well, or reset only zoom?
	 */
	void resetGameView(sf::RenderWindow& window, bool resetLocation);

	/**
	 * Open the settings menu.
	 *
	 * @param window The window of the event.
	 */
	void openSettingsMenu(sf::RenderWindow& window);
};

#endif

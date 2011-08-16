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
#include <vector>

#include "gui/game/GameObject.hpp"
#include "gui/widget/Container.hpp"
#include "gui/graphics/ImageCache.hpp"
#include "gui/menu/SettingsMenu.hpp"
#include "gui/game/ScrollingView.hpp"
#include "gui/game/MouseTracker.hpp"

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
	GUI::ScrollingView gameView;

	/** Mouse position */
	MouseTracker mouse;

	/** Positions where the mouse buttons went down. */
	std::map<sf::Mouse::Button, Vector2<SIUnit::Position> > mouseDownPosition;

	/** Used to break the main loop from within member functions. */
	bool gameClosed;

	/** The settings menu, if opened. */
	boost::shared_ptr<Menu::SettingsMenu> settingsMenu;

	/** Container type for lists of objects. */
	typedef std::vector<boost::shared_ptr<GameObject> > ObjectListType;

	/** Container for keeping track of selected objects */
	ObjectListType selectedObjects;

	/** Container type for mapping logical objects to GUI objects. */
	typedef std::map<boost::shared_ptr<World::Object>, boost::shared_ptr<GameObject> > GameObjectListType;

	/** Map to keep track of GameObjects. */
	GameObjectListType gameObjects;

	/**
	 * Get a GameObject for the given object.
	 *
	 * @param object The logical object.
	 * @return The GUI object.
	 */
	boost::shared_ptr<GameObject> getGameObject(boost::shared_ptr<World::Object> object);

	/**
	 * Get a list of the objects within the given range of the given coordinates.
	 *
	 * @param position The position to check.
	 * @param range The maximum range to accept.
	 */
	ObjectListType getObjectsWithinRange(Vector2<SIUnit::Position> position, Scalar<SIUnit::Length> range);

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
	 * Update the widget state, if it needs to change without events.
	 *
	 * @param window The current window.
	 */
	void updateState(sf::RenderWindow& window);

	/**
	 * Draw the widget.
	 *
	 * @param window The window to draw to.
	 */
	void draw(sf::RenderWindow& window);

	/**
	 * Open the settings menu.
	 *
	 * @param window The window of the event.
	 */
	void openSettingsMenu(sf::RenderWindow& window);
};

#endif

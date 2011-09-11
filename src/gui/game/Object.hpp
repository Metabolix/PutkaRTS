/*
 * GUI::GameObject class definition.
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

#ifndef PUTKARTS_GUI_Game_Object_HPP
#define PUTKARTS_GUI_Game_Object_HPP

#include <boost/shared_ptr.hpp>

#include <SFML/Graphics.hpp>

namespace GUI {
	namespace Game {
		class Object;
	}
}

namespace Game {
	class Object;
}

/**
 * GUI class that wraps a game object and handles drawing it.
 */
class GUI::Game::Object {
	/** The real object in the game. */
	boost::shared_ptr<const ::Game::Object> object;

public:
	/**
	 * Constructor.
	 *
	 * @param object The real object in the game.
	 */
	Object(boost::shared_ptr<const ::Game::Object> object);

	/* Access the object. */
	const boost::shared_ptr<const ::Game::Object>& getObject() const {
		return object;
	}

	/**
	 * Draw the object.
	 *
	 * @param window The window to use for rendering.
	 * @param selected Is this object selected?
	 */
	void draw(sf::RenderWindow& window, bool selected = false);
};

#endif

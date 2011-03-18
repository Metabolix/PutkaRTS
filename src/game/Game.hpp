/*
 * Game class definition.
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

#ifndef PUTKARTS_Game_HPP
#define PUTKARTS_Game_HPP

#include "util/Scalar.hpp"
#include "Message.hpp"
#include "Map.hpp"
#include "Object.hpp"

#include <map>
#include <memory>

/**
 * The root of all game logic.
 */
class Game {
public:
	friend class Map;
	friend class ObjectAction;

	/** Object type for storing the game's objects. */
	typedef std::map<unsigned int, boost::shared_ptr<World::Object> > ObjectContainerType;
private:
	/** Keep track of game time. */
	Scalar<SIUnit::Time> clock;

	/** The map on which the game is played. */
	std::auto_ptr<Map> map;

	/** Objects in the game */
	ObjectContainerType objects;

	/**
	 * Run the game one step forward.
	 *
	 * @param dt The time step length.
	 */
	void runStep(Scalar<SIUnit::Time> dt);
public:
	/**
	 * Constructor.
	 *
	 * @param map The map.
	 */
	Game(std::auto_ptr<Map> map);

	/**
	 * Get the map.
	 */
	const Map& getMap() const {
		return *map;
	}

	/**
	 * Get objects.
	 */
	const ObjectContainerType& getObjects() const {
		return objects;
	}

	/**
	 * Run the game up to the given time.
	 *
	 * @param time The time.
	 */
	void runUntil(Scalar<SIUnit::Time> time);

	/**
	 * Handle a message.
	 *
	 * @param message The message; timestamp will be overridden!
	 */
	void handleMessage(const Message& message);
protected:
	/**
	 * Add object to game.
	 *
	 * @param object Object to add.
	 */
	void insertObject(boost::shared_ptr<World::Object> object);
};

#endif

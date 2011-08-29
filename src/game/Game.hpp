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

#ifndef PUTKARTS_Game_Game_HPP
#define PUTKARTS_Game_Game_HPP

#include "util/Scalar.hpp"
#include "Message.hpp"
#include "Map.hpp"
#include "Object.hpp"

#include <queue>
#include <map>
#include <boost/shared_ptr.hpp>

#include <boost/function.hpp>

namespace Game {
	class Game;
}

/**
 * The root of all game logic.
 */
class Game::Game {
public:
	friend class Map;
	friend class ObjectAction;

	/** Object type for storing the game's objects. */
	typedef std::map<Object::IdType, boost::shared_ptr<Object> > ObjectContainerType;

	/** Type for specifying an external callback for message handling. */
	typedef boost::function<void(const Message&)> MessageCallbackType;
private:
	/** Keep track of game time. */
	Scalar<SIUnit::Time> clock;

	/** Pending messages. */
	std::priority_queue<Message> messages;

	/** The map on which the game is played. */
	boost::shared_ptr<Map> map;

	/** Objects in the game */
	ObjectContainerType objects;

	/**
	 * Run the game one step forward.
	 *
	 * @param dt The time step length.
	 * @param messageCallback The function to call when a message is handled.
	 */
	void runStep(Scalar<SIUnit::Time> dt, MessageCallbackType messageCallback);

	/**
	 * Handle the messages up to the current game time.
	 *
	 * @param messageCallback The function to call when a message is handled.
	 */
	void handleMessages(MessageCallbackType messageCallback);

	/**
	 * Handle a message.
	 *
	 * @param message The message to handle.
	 * @return true if the message is ok, false if it's somehow invalid.
	 */
	bool handleMessage(Message& message);
public:
	/**
	 * Constructor.
	 *
	 * @param map The map.
	 */
	Game(boost::shared_ptr<Map> map);

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
	 * @param messageCallback The function to call when a message is handled.
	 */
	void runUntil(Scalar<SIUnit::Time> time, MessageCallbackType messageCallback = 0);

	/**
	 * Insert a message in the queue.
	 *
	 * @param message The message.
	 */
	void insertMessage(const Message& message);
protected:
	/**
	 * Add object to game.
	 *
	 * @param object Object to add.
	 */
	void insertObject(boost::shared_ptr<Object> object);
};

#endif

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

#include <queue>
#include <memory>
#include <boost/unordered_map.hpp>
#include <boost/function.hpp>

#include "util/Scalar.hpp"
#include "lua/Lua.hpp"
#include "Message.hpp"
#include "Task.hpp"
#include "Map.hpp"
#include "Client.hpp"
#include "Player.hpp"
#include "ObjectType.hpp"
#include "ObjectAction.hpp"
#include "Object.hpp"

namespace Game {
	class Game;
}

/**
 * The root of all game logic.
 */
class Game::Game: protected Lua {
	friend class Map;
	friend class ObjectAction;

public:
	/** Type for client container. */
	typedef boost::unordered_map<Client::IdType, std::shared_ptr<Client> > ClientContainerType;

	/** Type for player container */
	typedef boost::unordered_map<Player::IdType, std::shared_ptr<Player> > PlayerContainerType;

	/** Type for storing the game's object types. */
	typedef boost::unordered_map<ObjectType::IdType, std::shared_ptr<const ObjectType> > ObjectTypeContainerType;

	/** Type for storing the game's object actions. */
	typedef boost::unordered_map<ObjectAction::IdType, std::shared_ptr<const ObjectAction> > ObjectActionContainerType;

	/** Object type for storing the game's objects. */
	typedef boost::unordered_map<Object::IdType, std::shared_ptr<Object> > ObjectContainerType;

	/** Type for specifying an external callback for message handling. */
	typedef boost::function<void(const Message&)> MessageCallbackType;

private:
	/** Keep track of game time. */
	Scalar<SIUnit::Time> clock;

	/** Pending messages. */
	std::priority_queue<Message> messages;

	/** The map on which the game is played. */
	std::shared_ptr<Map> map;

	/** Clients (physical people in the game). */
	ClientContainerType clients;

	/** Players (logical people on the map). */
	PlayerContainerType players;

	/** Object types. */
	ObjectTypeContainerType objectTypes;

	/** Object actions. */
	ObjectActionContainerType objectActions;

	/** Next id to assign to an object. */
	Object::IdType freeObjectId;

	/** Objects in the game */
	ObjectContainerType objects;

	/** A task for idle units. */
	const std::shared_ptr<Task> idleTask;

private:
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
	Game(std::shared_ptr<Map> map);

	/**
	 * Get the current time.
	 */
	Scalar<SIUnit::Time> getTime() const {
		return clock;
	}

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
	 * Get players.
	 */
	const PlayerContainerType& getPlayers() const {
		return players;
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

	/**
	 * Insert a new client.
	 *
	 * @param client The client.
	 */
	void insertClient(std::shared_ptr<Client> client);

	/**
	 * Remove a client.
	 */
	void eraseClient(int id);

protected:
	/**
	 * Remove an object from the game.
	 *
	 * @param object Object to remove.
	 */
	void eraseObject(std::shared_ptr<Object> object);

	/**
	 * Add new player.
	 *
	 * @param player Pointer to the player object to be added.
	 */
	void insertPlayer(std::shared_ptr<Player> player);

	/**
	 * Lua callback: Add an object type.
	 */
	void luaNewObjectType();

	/**
	 * Lua callback: Add an object action.
	 */
	void luaNewObjectAction();

	/**
	 * Lua callback: Add an object.
	 */
	void luaNewObject();

	/**
	 * Lua callback: Delete an object.
	 */
	void luaDeleteObject();
};

#endif

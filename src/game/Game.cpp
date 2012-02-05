/*
 * Game class implementation.
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

#include <stdexcept>
#include <vector>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>

#include "Game.hpp"

Game::Game::Game(boost::shared_ptr<Map> map_):
	map(map_),
	freeObjectId(1) {
	if (!map.get()) {
		throw std::logic_error("Game::Game: Map is NULL!");
	}

	// Initialise the Lua interface.
	bind("luaNewObjectType", boost::bind(&Game::luaNewObjectType, this));
	bind("luaNewObjectAction", boost::bind(&Game::luaNewObjectAction, this));
	bind("luaNewObject", boost::bind(&Game::luaNewObject, this));
	bind("luaDeleteObject", boost::bind(&Game::luaDeleteObject, this));
	runFile<void>(Path::findDataPath("lua/Game.lua"));

	// TODO: Read the tech tree.
	run<void>("\
		ObjectType.new({\
			id = 'testStart',\
			name = 'Test starting position',\
			new = function(t)\
				for dx = -1, 1 do for dy = -1, 1 do Object.new({\
					objectTypeId = (dx == 0 and dy == 0 and 'testUnit2') or 'testUnit',\
					playerId = t.playerId,\
					x = t.x + dx,\
					y = t.y + dy,\
				}) end end\
				Object.delete(t)\
			end\
		})\
		ObjectType.new({id = 'testUnit', name = 'Test unit', radius = 0.4, maxVelocity = 2.5})\
		ObjectType.new({id = 'testUnit2', name = 'Bigger test unit', radius = 0.6, maxVelocity = 3.5})\
	");

	// Initialise players.
	const Map::PlayerContainerType& mapPlayers = map->getPlayers();
	for (Map::PlayerContainerType::const_iterator i = mapPlayers.begin(); i != mapPlayers.end(); ++i) {
		const Map::Player& p = i->second;

		boost::shared_ptr<Player> testPlayer(new Player);
		testPlayer->name = std::string("Player ") + (char)('1' + players.size());
		insertPlayer(testPlayer);

		// Create starting position.
		load("local t = {...}; Object.new({objectTypeId = 'testStart', playerId = t[1], x = t[2], y = t[3]});");
		push<Lua::Number>(testPlayer->id);
		push<Lua::Number>(p.startPosition.x.getDouble());
		push<Lua::Number>(p.startPosition.y.getDouble());
		call(3, 0);
	}
}

void Game::Game::runUntil(Scalar<SIUnit::Time> time, MessageCallbackType messageCallback) {
	const Scalar<SIUnit::Time> dt = 1.0 / 32;
	while (clock + dt <= time) {
		runStep(dt, messageCallback);
	}
}

void Game::Game::insertMessage(const Message& message) {
	messages.push(message);
}

void Game::Game::eraseObject(boost::shared_ptr<Object> object) {
	load("if Game.objects[...] then Object.delete(Game.objects[...]) end");
	push<Lua::Number>(object->id);
	call(1, 0);
}

void Game::Game::runStep(Scalar<SIUnit::Time> dt, MessageCallbackType messageCallback) {
	clock += dt;
	handleMessages(messageCallback);

	typedef std::vector<boost::shared_ptr<Object> > ObjectVectorType;
	ObjectVectorType tmp;
	tmp.reserve(objects.size());
	for (ObjectContainerType::iterator i = objects.begin(); i != objects.end(); ++i) {
		tmp.push_back(i->second);
	}
	for (ObjectVectorType::iterator i = tmp.begin(); i != tmp.end(); ++i) {
		Object& object = **i;
		if (!object.runStep(dt, *this)) {
			eraseObject(*i);
		}
	}
}

bool Game::Game::handleMessage(Message& message) {
	// Check the sender of the message.
	if (clients.find(message.client) == clients.end()) {
		return false;
	}

	boost::shared_ptr<Client> client(clients[message.client]);
	boost::shared_ptr<Task> task(new Task);

	// Create a dummy target for moving, if needed.
	if (message.action == ObjectAction::MOVE && message.targets.empty()) {
		task->dummy.reset(new Object(message.position));
		task->targets.push_back(task->dummy);
	}

	// Check the action.
	if (objectActions.find(message.action) != objectActions.end()) {
		task->action = objectActions[message.action];
	}
	if (!task->action && message.action != ObjectAction::MOVE && message.action != ObjectAction::DELETE) {
		return false;
	}

	// Collect actors that exist and are allowed.
	BOOST_FOREACH(Object::IdType id, message.actors) {
		if (objects.find(id) == objects.end()) {
			continue;
		}
		boost::shared_ptr<Object> object(objects[id]);
		if (client->players.find(object->owner->id) == client->players.end()) {
			continue;
		}
		// Replace the current task with the new one.
		if (object->task) {
			for (std::list<boost::weak_ptr<Object> >::iterator i = object->task->actors.begin(); i != object->task->actors.end(); ++i) {
				if (i->lock() == object) {
					object->task->actors.erase(i);
					break;
				}
			}
		}
		object->task = task;
		task->actors.push_back(object);
	}

	if (task->actors.empty()) {
		return false;
	}

	// Collect targets.
	BOOST_FOREACH(Object::IdType id, message.targets) {
		if (objects.find(id) == objects.end()) {
			continue;
		}
		task->targets.push_back(objects[id]);
	}

	return true;
}

void Game::Game::handleMessages(MessageCallbackType messageCallback) {
	while (!messages.empty() && messages.top().timestamp <= clock) {
		Message message(messages.top());
		messages.pop();

		message.timestamp = clock;
		if (handleMessage(message) && messageCallback) {
			messageCallback(message);
		}
	}
}

void Game::Game::insertPlayer(boost::shared_ptr<Player> player) {
	player->id = players.size() + 1;
	players.insert(std::make_pair(player->id, player));
}

void Game::Game::insertClient(boost::shared_ptr<Client> client) {
	clients[client->id] = client;
}

void Game::Game::eraseClient(int id) {
	clients.erase(id);
}

void Game::Game::luaNewObjectType() {
	boost::shared_ptr<ObjectType> tmp(new ObjectType);
	tmp->id = get<String>(1);
	tmp->name = get<String>(2);
	tmp->immutable = get<Boolean>(3);
	tmp->radius = get<Number>(4);
	tmp->maxVelocity = get<Number>(5);
	tmp->lineOfSight = get<Number>(6);
	tmp->maxHitPoints = get<Number>(7);
	objectTypes[tmp->id] = tmp;
}

void Game::Game::luaNewObjectAction() {
	boost::shared_ptr<ObjectAction> tmp(new ObjectAction);
	tmp->id = get<String>(1);
	tmp->name = get<String>(2);
	objectActions[tmp->id] = tmp;
}

void Game::Game::luaNewObject() {
	if (freeObjectId <= 0) {
		throw std::runtime_error("FIXME: freeObjectId has overflown!");
	}

	boost::shared_ptr<Object> tmp(new Object(Vector2<SIUnit::Position>(get<Number>(3), get<Number>(4))));
	tmp->objectType = objectTypes[get<String>(1)],
	tmp->owner = players[get<Number>(2)],
	tmp->id = freeObjectId++;
	objects[tmp->id] = tmp;
	push<Number>(tmp->id);
}

void Game::Game::luaDeleteObject() {
	Object::IdType id = get<Number>(1);
	if (objects.find(id) == objects.end()) {
		return;
	}
	objects[id]->dead = true;
	objects.erase(id);
}

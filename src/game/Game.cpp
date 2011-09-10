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

#include "Game.hpp"

#include <boost/bind.hpp>
#include <stdexcept>
#include <vector>

Game::Game::Game(boost::shared_ptr<Map> map_):
	map(map_),
	freeObjectId(1) {
	if (!map.get()) {
		throw std::logic_error("Game::Game: Map is NULL!");
	}

	// Function for defining object types, and a wrapper for simpler syntax.
	bind("newObjectType", boost::bind(&Game::luaNewObjectType, this));
	run<void>(
		"function type(t) return newObjectType("
		"t.id or 'dummy', "
		"t.name or 'Unknown', "
		"t.immutable or false, "
		"t.radius or 0.5, "
		"t.maxVelocity or 0, "
		"t.lineOfSight or 10, "
		"t.maxHitPoints or 0"
		") end"
	);

	// Create some units for testing.
	run<void>("type({id = 'test', name = 'Test type', maxVelocity = 2.5})");
	boost::shared_ptr<const ObjectType> testObjectType(objectTypes.begin()->second);

	const Map::PlayerContainerType& mapPlayers = map->getPlayers();
	for (Map::PlayerContainerType::const_iterator i = mapPlayers.begin(); i != mapPlayers.end(); ++i) {
		const Map::Player& p = i->second;

		boost::shared_ptr<Player> testPlayer(new Player(std::string("Player ") + (char)('1' + players.size())));
		insertPlayer(testPlayer);

		for (int dx = -1; dx <= 1; ++dx) {
			for (int dy = -1; dy <= 1; ++dy) {
				insertObject(boost::shared_ptr<Object>(new Object(testObjectType, testPlayer, p.startPosition + Vector2<SIUnit::Position>(dx, dy))));
			}
		}
	}
}

void Game::Game::runUntil(Scalar<SIUnit::Time> time, MessageCallbackType messageCallback) {
	const Scalar<SIUnit::Time> dt = 1.0 / 32;
	while (clock + dt < time) {
		runStep(dt, messageCallback);
	}
}

void Game::Game::insertMessage(const Message& message) {
	messages.push(message);
}

void Game::Game::insertObject(boost::shared_ptr<Object> object) {
	if (freeObjectId <= 0) {
		throw std::runtime_error("FIXME: freeObjectId has overflown!");
	}
	object->id = freeObjectId++;
	objects.insert(std::make_pair(object->id, object));
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
			objects.erase(object.id);
		}
	}
}

bool Game::Game::handleMessage(Message& message) {
	// TODO: Check that the message is valid!
	bool notValid = false;
	if (notValid) {
		return false;
	}

	// Collect actors that exist.
	ObjectContainerType actors;
	for (std::list<Object::IdType>::iterator i = message.actors.begin(); i != message.actors.end(); ++i) {
		if (objects.find(*i) != objects.end()) {
			actors.insert(*objects.find(*i));
		}
	}

	// Hard-coded action: NEW.
	if (message.action == ObjectAction::NEW) {
		return true;
	}

	// Hard-coded action: DELETE.
	if (message.action == ObjectAction::DELETE) {
		for (ObjectContainerType::iterator i = actors.begin(); i != actors.end(); ++i) {
			objects.erase(i->first);
		}
		return true;
	}

	// Hard-coded action: MOVE.
	if (message.action == ObjectAction::MOVE) {
		for (ObjectContainerType::iterator i = actors.begin(); i != actors.end(); ++i) {
			i->second->targetPosition = message.position;
		}
		return true;
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

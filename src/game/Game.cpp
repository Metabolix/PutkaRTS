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

#include <stdexcept>

Game::Game(std::auto_ptr<Map> map_):
	map(map_) {
	if (!map.get()) {
		throw std::logic_error("Game::Game: Map is NULL!");
	}

	map->createInitialObjects(*this);
}

void Game::runUntil(Scalar<SIUnit::Time> time, MessageCallbackType messageCallback) {
	const Scalar<SIUnit::Time> dt = 1.0 / 32;
	while (clock + dt < time) {
		runStep(dt, messageCallback);
	}
}

void Game::insertMessage(const Message& message) {
	messages.push(message);
}

void Game::insertObject(boost::shared_ptr<World::Object> object) {
	//  FIXME: This might overflow in long games.
	object->id = 1 + (objects.empty() ? 0 : objects.rbegin()->first);
	objects.insert(std::make_pair(object->id, object));
}

void Game::runStep(Scalar<SIUnit::Time> dt, MessageCallbackType messageCallback) {
	clock += dt;
	handleMessages(messageCallback);

	for (ObjectContainerType::iterator i = objects.begin(); i != objects.end();) {
		ObjectContainerType::iterator next = i;
		++next;
		if (!i->second->runStep(dt, *this)) {
			objects.erase(i);
		}
		i = next;
	}
}

bool Game::handleMessage(Message& message) {
	// TODO: Check that the message is valid!
	bool notValid = false;
	if (notValid) {
		return false;
	}

	// Remove actors that do not exist.
	for (std::list<World::Object::IdType>::iterator j, i = message.actors.begin(); i != message.actors.end();) {
		j = i++;
		if (objects.find(*j) == objects.end()) {
			i = message.actors.erase(j);
		}
	}

	// TODO: Do something with the message!
	return true;
}

void Game::handleMessages(MessageCallbackType messageCallback) {
	while (!messages.empty() && messages.top().timestamp <= clock) {
		Message message(messages.top());
		messages.pop();

		message.timestamp = clock;
		if (handleMessage(message) && messageCallback) {
			messageCallback(message);
		}
	}
}

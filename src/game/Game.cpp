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

void Game::runUntil(Scalar<SIUnit::Time> time) {
	const Scalar<SIUnit::Time> dt = 1.0 / 32;
	while (clock + dt < time) {
		runStep(dt);
	}
}

void Game::insertObject(boost::shared_ptr<World::Object> object) {
	//  FIXME: This might overflow in long games.
	unsigned int id = objects.empty() ? 0 : objects.rbegin()->first;
	objects.insert(std::make_pair(id + 1, object));
}

void Game::runStep(Scalar<SIUnit::Time> dt) {
	clock += dt;
	// TODO: Handle messages!

	for (ObjectContainerType::iterator i = objects.begin(); i != objects.end();) {
		ObjectContainerType::iterator next = i;
		++next;
		if (!i->second->runStep(dt, *this)) {
			objects.erase(i);
		}
		i = next;
	}
}

void Game::handleMessage(const Message& message) {
	// TODO: Copy the message into an internal list, set message.timestamp = clock;
}

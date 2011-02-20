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

const Scalar<SIUnit::Time> Game::stepTime = 1 / 30;

Game::Game(std::auto_ptr<Map> map_):
	map(map_) {
	if (!map.get()) {
		throw std::logic_error("Game::Game: Map is NULL!");
	}
}

void Game::runUntil(Scalar<SIUnit::Time> time) {
	while (clock + stepTime < time) {
		runStep();
	}
}

void Game::runStep() {
	// TODO: Update clock by some value and update the world respectively.
}

void Game::handleMessage(const Message& message) {
	// TODO: Copy the message into an internal list, set message.timestamp = clock;
}

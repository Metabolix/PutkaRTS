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

#include <memory>

/**
 * The root of all game logic.
 */
class Game {
	/** Game step duration. */
	static const Scalar<SIUnit::Time> stepTime;

	/** Keep track of game time. */
	Scalar<SIUnit::Time> clock;

	/** The map on which the game is played. */
	std::auto_ptr<Map> map;

	/**
	 * Run the game one step forward.
	 */
	void runStep();
public:
	/**
	 * Constructor.
	 *
	 * @param map The map.
	 */
	Game(std::auto_ptr<Map> map);

	/**
	 * Run the game up to the given time.
	 *
	 * @param time The time.
	 */
	const Map& getMap() const {
		return *map;
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
};

#endif

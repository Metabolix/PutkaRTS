/*
 * Server side of game communication.
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

#ifndef PUTKARTS_Connection_Server_HPP
#define PUTKARTS_Connection_Server_HPP

#include <string>

#include "game/Message.hpp"
#include "connection/Base.hpp"

#include "SFML/System.hpp" // FIXME: Get rid of this!

namespace Connection {
	class Server;
}

/**
 * Base class for game servers.
 */
class Connection::Server: virtual public Connection::Base {
	/** The clock used for timing the game. */
	sf::Clock clock;

public:
	/**
	 * Run the game up to this moment.
	 */
	void runUntilNow();

	/**
	 * Start the clock.
	 */
	virtual void startGame();
};

#endif

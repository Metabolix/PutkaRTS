/*
 * Client side of game communication.
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

#ifndef PUTKARTS_Connection_Client_HPP
#define PUTKARTS_Connection_Client_HPP

#include <string>

#include "connection/Base.hpp"

namespace Connection {
	class Client;
}

/**
 * Base class for game clients.
 */
class Connection::Client: virtual public Connection::Base {
public:
	/**
	 * Send a message describing a player action.
	 *
	 * @param message The message to send.
	 */
	virtual void sendMessage(const Game::Message& message) = 0;

	/**
	 * Mark the client as ready for initialising the game ("settings ok, let's play").
	 */
	virtual void setReadyToInit() = 0;

	/**
	 * Mark the client as ready for initialising the game ("all set, start the clock").
	 */
	virtual void setReadyToStart() = 0;
};

#endif

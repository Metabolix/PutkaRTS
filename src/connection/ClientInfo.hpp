/*
 * Information about one client.
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

#ifndef PUTKARTS_Connection_ClientInfo_HPP
#define PUTKARTS_Connection_ClientInfo_HPP

#include <string>

#include "game/Client.hpp"

namespace Connection {
	struct ClientInfo;
}

/**
 * Information about one client.
 */
struct Connection::ClientInfo: Game::Client {
	/** Is this client ready to init the game? */
	bool readyToInit;

	/** Is this client ready to start playing? */
	bool readyToStart;

	/** Constructor; sets the default values. */
	ClientInfo():
		readyToInit(false),
		readyToStart(false) {
	}

	/**
	 * Virtual destructor.
	 */
	virtual ~ClientInfo() {}

	/**
	 * Create a client info from string.
	 */
	ClientInfo(const std::string& data);

	/**
	 * Conver a client info to string.
	 */
	std::string serialize() const;
};

#endif
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

namespace Connection {
	struct ClientInfo;
}

/**
 * Information about one client.
 */
struct Connection::ClientInfo {
	/** Constructor; sets the default values. */
	ClientInfo():
		id(0),
		name("Unknown"),
		ai(false),
		readyToInit(false),
		readyToStart(false) {
	}

	/**
	 * Virtual destructor.
	 */
	virtual ~ClientInfo() {}

	/** The client id set by the server. */
	int id;

	/** The name of this client. */
	std::string name;

	/** Is this a computer player? */
	bool ai;

	/** Is this client ready to init the game? */
	bool readyToInit;

	/** Is this client ready to start playing? */
	bool readyToStart;
};

#endif

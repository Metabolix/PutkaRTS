/*
 * Interface for listeners.
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

#ifndef PUTKARTS_Connection_Listener_HPP
#define PUTKARTS_Connection_Listener_HPP

#include <boost/utility.hpp>

namespace Connection {
	class Server;
	class Listener;
}

/**
 * Interface for listeners.
 */
class Connection::Listener: boost::noncopyable {
public:
	/**
	 * Virtual destructor.
	 */
	virtual ~Listener() {
		// Nothing to do.
	}

	/**
	 * Inform the server about changes.
	 *
	 * @param server The server.
	 * @return false if this Listener should be removed, true otherwise.
	 */
	virtual bool update(Server& server) = 0;
};

#endif

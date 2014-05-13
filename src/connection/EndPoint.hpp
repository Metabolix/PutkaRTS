/*
 * Connection end point.
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

#ifndef PUTKARTS_Connection_EndPoint_HPP
#define PUTKARTS_Connection_EndPoint_HPP

#include <string>
#include <memory>

namespace Connection {
	class EndPoint;
}

/**
 * Connection end point.
 */
class Connection::EndPoint {
public:
	/**
	 * Virtual destructor.
	 */
	virtual ~EndPoint() {
		// Nothing to do.
	}

	/**
	 * Send a data packet (message).
	 *
	 * @param data The message to send.
	 */
	virtual void sendPacket(const std::string& data) = 0;

	/**
	 * Receive a data packet (message).
	 *
	 * @param data The message is stored here.
	 * @return true if a packet was received, false otherwise.
	 */
	virtual bool receivePacket(std::string& data) = 0;
};

#endif

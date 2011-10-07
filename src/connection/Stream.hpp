/*
 * Connection helper that wraps packets into a stream.
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

#ifndef PUTKARTS_Connection_Stream_HPP
#define PUTKARTS_Connection_Stream_HPP

#include <vector>

#include "connection/EndPoint.hpp"

namespace Connection {
	class Stream;
}

/**
 * Connection helper that wraps packets into a stream.
 */
class Connection::Stream: public Connection::EndPoint {
	/** Number of bytes missing from the next packet. */
	size_t recvSize;

protected:
	/** The buffer for receiving data. */
	std::vector<char> recvBuf;

	/**
	 * Constructor.
	 */
	Stream():
		recvSize(0) {
	}

	/**
	 * Send some data.
	 *
	 * @param data The data to send.
	 */
	virtual void sendData(const std::string& data) = 0;

	/**
	 * Receive some data and append it to the recvBuf.
	 *
	 * @param size The maximum amount of data to receive.
	 */
	virtual void receiveData(size_t size) = 0;

public:
	/** @copydoc EndPoint::sendPacket */
	virtual void sendPacket(const std::string& data);

	/** @copydoc EndPoint::receivePacket */
	virtual bool receivePacket(std::string& data);
};

#endif

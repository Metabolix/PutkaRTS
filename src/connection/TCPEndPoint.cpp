/*
 * TCP connection.
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

#include <boost/format.hpp>

#include "TCPEndPoint.hpp"

void Connection::TCPEndPoint::sendData(const std::string& data) {
	boost::asio::write(socket, boost::asio::buffer(data));
}

void Connection::TCPEndPoint::receiveData(size_t size) {
	if (!socket.available()) {
		return;
	}
	size_t old = recvBuf.size();
	recvBuf.resize(old + size);
	recvBuf.resize(old + socket.read_some(boost::asio::buffer(&recvBuf[old], size)));
}
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

void Connection::TCPEndPoint::sendPacket(const std::string& data) {
	std::string tmp = (boost::format("%08x") % data.size()).str();
	boost::asio::write(socket, boost::asio::buffer(tmp));
	boost::asio::write(socket, boost::asio::buffer(data));
}

bool Connection::TCPEndPoint::receivePacket(std::string& data) {
	while (!recvSize) {
		if (socket.available() < 8) {
			return false;
		}
		char tmp[9] = {0};
		boost::asio::read(socket, boost::asio::buffer(tmp, 8));
		recvSize = std::strtoul(tmp, 0, 16);
		recvBuf.resize(recvSize);
	}
	if (!socket.available()) {
		return false;
	}
	recvSize -= socket.read_some(boost::asio::buffer(&recvBuf[recvBuf.size() - recvSize], recvSize));
	if (recvSize > 0) {
		return false;
	}
	data.assign(recvBuf.begin(), recvBuf.end());
	return true;
}

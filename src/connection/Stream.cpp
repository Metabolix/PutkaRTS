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

#include <boost/format.hpp>

#include "Stream.hpp"

void Connection::Stream::sendPacket(const std::string& data) {
	sendData((boost::format("%08x") % data.size()).str());
	sendData(data);
}

bool Connection::Stream::receivePacket(std::string& data) {
	while (!recvSize) {
		receiveData(8 - recvBuf.size());
		if (recvBuf.size() < 8) {
			return false;
		}
		recvBuf.push_back(0);
		recvSize = std::strtoul(&recvBuf[0], 0, 16);
		recvBuf.clear();
		recvBuf.reserve(recvSize);
	}
	receiveData(recvSize - recvBuf.size());
	if (recvBuf.size() < recvSize) {
		return false;
	}
	data.assign(recvBuf.begin(), recvBuf.end());
	recvBuf.clear();
	recvSize = 0;
	return true;
}

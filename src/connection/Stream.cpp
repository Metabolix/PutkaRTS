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

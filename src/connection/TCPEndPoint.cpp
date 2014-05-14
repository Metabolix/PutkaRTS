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

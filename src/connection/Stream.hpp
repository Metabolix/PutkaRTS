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

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

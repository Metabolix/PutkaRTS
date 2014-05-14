#ifndef PUTKARTS_Connection_TCPEndPoint_HPP
#define PUTKARTS_Connection_TCPEndPoint_HPP

#include <vector>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>

#include "connection/Stream.hpp"

namespace Connection {
	class TCPEndPoint;
}

/**
 * Connection end point.
 */
class Connection::TCPEndPoint: public Connection::Stream {
	friend class TCPListener;

	/** The io service. */
	boost::asio::io_service service;

	/** The TCP stream. */
	boost::asio::ip::tcp::socket socket;

public:
	/**
	 * Default constructor for use with TCPListener.
	 */
	TCPEndPoint():
		socket(service) {
	}

	/**
	 * Connect to an address.
	 *
	 * @param address The address.
	 * @param port The port.
	 */
	TCPEndPoint(const std::string& address, int port):
		socket(service) {
		connect(address, boost::lexical_cast<std::string>(port));
	}

	/**
	 * Connect to an address.
	 *
	 * @param address The address.
	 * @param port The port.
	 */
	TCPEndPoint(const std::string& address, const std::string& port):
		socket(service) {
		connect(address, port);
	}

	/**
	 * Connect to an address.
	 *
	 * @param address The address.
	 * @param port The port.
	 */
	void connect(const std::string& address, const std::string& port) {
		boost::asio::ip::tcp::resolver resolver(service);
		boost::asio::ip::tcp::resolver::query query(address, port);
		boost::asio::connect(socket, resolver.resolve(query));
	}

protected:
	/** @copydoc Stream::sendData */
	virtual void sendData(const std::string& data);

	/** @copydoc Stream::receiveData */
	virtual void receiveData(size_t size);
};

#endif

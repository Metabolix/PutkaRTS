#ifndef PUTKARTS_Connection_TCPListener_HPP
#define PUTKARTS_Connection_TCPListener_HPP

#include <memory>
#include <boost/asio.hpp>

#include "connection/Server.hpp"
#include "connection/TCPEndPoint.hpp"

namespace Connection {
	class TCPListener;
}

/**
 * TCP listener.
 */
class Connection::TCPListener: public Connection::Listener {
	friend class Address;

	/** Boost IO service. */
	boost::asio::io_service service;

	/** The listening socket. */
	boost::asio::ip::tcp::acceptor acceptor;

	/** A placeholder for the next client. */
	std::shared_ptr<TCPEndPoint> next;

public:
	/**
	 * Construct a new listener at the given address.
	 *
	 * @param address The address.
	 */
	TCPListener(const boost::asio::ip::tcp::endpoint& address);

	/** @copydoc Connection::Listener::update */
	virtual bool update(Server& server);
};

#endif

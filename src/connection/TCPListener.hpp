/*
 * TCP listener.
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

#ifndef PUTKARTS_Connection_TCPListener_HPP
#define PUTKARTS_Connection_TCPListener_HPP

#include <boost/shared_ptr.hpp>
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
	boost::shared_ptr<TCPEndPoint> next;

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

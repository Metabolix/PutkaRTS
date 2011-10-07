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

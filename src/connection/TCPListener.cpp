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

#include <boost/format.hpp>

#include "TCPListener.hpp"

Connection::TCPListener::TCPListener(const boost::asio::ip::tcp::endpoint& address):
	acceptor(service),
	next(new TCPEndPoint()) {
	acceptor.open(address.protocol());
	acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	if (address.address().is_v6()) {
		acceptor.set_option(boost::asio::ip::v6_only(true));
	}
	acceptor.non_blocking(true);
	acceptor.bind(address);
	acceptor.listen();
}

bool Connection::TCPListener::update(Server& server) {
	while (true) {
		boost::system::error_code error;
		acceptor.accept(next->socket, error);
		if (error) {
			return (error == boost::asio::error::would_block);
		}
		server.addClient(next);
		next.reset(new TCPEndPoint());
	}
	return true;
}

/*
 * Functions for handling addresses.
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

#include <stdexcept>
#include <memory>
#include <boost/format.hpp>

#include "Address.hpp"

#include "TCPListener.hpp"
#include "TCPEndPoint.hpp"

std::shared_ptr<Connection::EndPoint> Connection::Address::connect(const std::string& str) {
	if (str.substr(0, 7) == "tcp4://") {
		return std::make_shared<TCPEndPoint>(str.substr(7, str.rfind(':') - 7), str.substr(str.rfind(':') + 1));
	}
	if (str.substr(0, 7) == "tcp6://") {
		return std::make_shared<TCPEndPoint>(str.substr(8, str.rfind(':') - 9), str.substr(str.rfind(':') + 1));
	}
	throw std::runtime_error("Unknown address type!");
}

std::string Connection::Address::toString(const Listener& listener) {
	if (const TCPListener* tcp = dynamic_cast<const TCPListener*>(&listener)) {
		boost::asio::ip::tcp::endpoint endpoint = tcp->acceptor.local_endpoint();
		boost::asio::ip::address address = endpoint.address();
		if (address.is_loopback()) {
			throw std::runtime_error("No address available!");
		}
		if (address.is_v6() && address.to_v6().is_v4_mapped()) {
			address = address.to_v6().to_v4();
		}
		if (address.is_v4()) {
			return (boost::format("tcp4://%s:%d") % address.to_v4().to_string() % endpoint.port()).str();
		}
		return (boost::format("tcp6://[%s]:%d") % address.to_v6().to_string() % endpoint.port()).str();
	}
	throw std::runtime_error("No address available!");
}

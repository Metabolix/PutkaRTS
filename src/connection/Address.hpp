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

#ifndef PUTKARTS_Connection_Address_HPP
#define PUTKARTS_Connection_Address_HPP

#include <string>
#include <memory>

namespace Connection {
	class EndPoint;
	class Listener;
	class Address;
}

class Connection::Address {
	/** Make the class non-constructible. */
	virtual void makeNonConstructible() = 0;

public:
	/**
	 * Connect to an address.
	 *
	 * @param address The address.
	 * @return A pointer to the connection.
	 * @throw std::exception Thrown if something goes wrong.
	 */
	static std::shared_ptr<Connection::EndPoint> connect(const std::string& address);

	/**
	 * Get the address of a listener.
	 *
	 * @param listener The listener.
	 * @return The address of the listener.
	 * @throw std::exception Thrown if something goes wrong.
	 */
	static std::string toString(const Listener& listener);
};

#endif

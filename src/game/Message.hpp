/*
 * Game message class definition.
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

#ifndef PUTKARTS_Message_HPP
#define PUTKARTS_Message_HPP

#include "util/Scalar.hpp"
#include "util/Vector2.hpp"

#include "Object.hpp"

#include <list>

/**
 * This class should describe any action in the game, most importantly moving units.
 */
class Message {
public:
	/** The timestamp for handling the message. */
	Scalar<SIUnit::Time> timestamp;

	/** The position associated with this message; mostly used for moving and such. */
	Vector2<SIUnit::Position> position;

	/** The actor objects. */
	std::list<World::Object::IdType> actors;

	/**
	 * Default constructor.
	 */
	Message() {
		// Nothing here.
	}

	/**
	 * Constructor that deserializes a message, usually received over the network.
	 *
	 * @param data The bytes representing a message.
	 */
	Message(std::string data);

	/**
	 * Serialize the message for sending it over the network.
	 *
	 * @return The bytes representing this message.
	 */
	std::string serialize() const;

	/**
	 * Compare this message to another; order by timestamps.
	 *
	 * @param m2 The other message
	 * @return true if this message has earlier timestamp than the other.
	 */
	bool operator < (const Message& m2) const {
		const Message& m1 = *this;
		return m1.timestamp < m2.timestamp;
	}
};

#endif

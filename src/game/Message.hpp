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

#ifndef PUTKARTS_Game_Message_HPP
#define PUTKARTS_Game_Message_HPP

#include <list>

#include "util/Scalar.hpp"
#include "util/Vector2.hpp"

#include "Client.hpp"
#include "Object.hpp"
#include "ObjectAction.hpp"

namespace Game {
	class Message;
}

/**
 * This class should describe any action in the game, most importantly moving units.
 */
class Game::Message {
public:
	/** The client who sent this message. */
	Client::IdType client;

	/** The timestamp for handling the message. */
	Scalar<SIUnit::Time> timestamp;

	/** The action. */
	ObjectAction::IdType action;

	/** The position associated with this message; mostly used for moving and such. */
	Vector2<SIUnit::Position> position;

	/** The actor objects. */
	std::list<Object::IdType> actors;

	/** The target objects. */
	std::list<Object::IdType> targets;

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
	Message(const std::string& data);

	/**
	 * Serialize the message for sending it over the network.
	 *
	 * @return The bytes representing this message.
	 */
	std::string serialize() const;

	/**
	 * Compare this message to another. The greatest message is the one
	 * with greatest priority (i.e. the smallest timestamp).
	 *
	 * @param m2 The other message
	 * @return true if this message is less important than the other.
	 */
	bool operator < (const Message& m2) const {
		const Message& m1 = *this;
		if (m1.timestamp != m2.timestamp) {
			return m1.timestamp > m2.timestamp;
		}
		if (m1.client != m2.client) {
			return m1.client < m2.client;
		}
		if (m1.action != m1.action) {
			return m1.action < m2.action;
		}
		if (m1.position.x != m2.position.x) {
			return m1.position.x < m2.position.x;
		}
		if (m1.position.y != m2.position.y) {
			return m1.position.y < m2.position.y;
		}
		if (m1.actors.size() != m2.actors.size()) {
			return m1.actors.size() < m2.actors.size();
		}
		for (std::list<Object::IdType>::const_iterator i = m1.actors.begin(), j = m2.actors.begin(); i != m1.actors.end(); ++i, ++j) {
			if (*i != *j) {
				return *i < *j;
			}
		}
		if (m1.targets.size() != m2.targets.size()) {
			return m1.targets.size() < m2.targets.size();
		}
		for (std::list<Object::IdType>::const_iterator i = m1.targets.begin(), j = m2.targets.begin(); i != m1.targets.end(); ++i, ++j) {
			if (*i != *j) {
				return *i < *j;
			}
		}
		return m1.serialize() < m2.serialize();
	}
};

#endif

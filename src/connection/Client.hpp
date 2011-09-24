/*
 * Client side of game communication.
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

#ifndef PUTKARTS_Connection_Client_HPP
#define PUTKARTS_Connection_Client_HPP

#include <string>

#include "util/Scalar.hpp"
#include "connection/Base.hpp"
#include "connection/EndPoint.hpp"

namespace Connection {
	class Client;
}

namespace Game {
	class Player;
}

/**
 * Base class for game clients.
 */
class Connection::Client: virtual public Connection::Base {
	/** Connection to the server. */
	boost::shared_ptr<EndPoint> connection;

	/** The client id of this client. */
	int ownId;

	/** The greatest message timestamp. */
	Scalar<SIUnit::Time> lastMessageTimestamp;

	/** The greatest message timestamp before lastMessageTimestamp. */
	Scalar<SIUnit::Time> prevMessageTimestamp;

protected:
	/**
	 * Handle a received packet.
	 *
	 * @param data The packet.
	 */
	void handlePacket(std::string& data);

public:
	/**
	 * Construct a new client with the specified connection.
	 *
	 * @param conn Connection to a server.
	 */
	Client(boost::shared_ptr<EndPoint> conn):
		connection(conn) {
	}

	/**
	 * Send a message describing a player action.
	 *
	 * @param message The message to send.
	 */
	virtual void sendMessage(const Game::Message& message);

	/**
	 * Handle data from the server, and update the game state.
	 */
	virtual void update();

	/**
	 * Mark the client as ready for initialising the game ("settings ok, let's play").
	 */
	void setReadyToInit();

	/**
	 * Mark the client as ready for initialising the game ("all set, start the clock").
	 */
	void setReadyToStart();

	/**
	 * Get the player that this client represents.
	 */
	boost::shared_ptr<const Game::Player> getPlayer() const;
};

#endif

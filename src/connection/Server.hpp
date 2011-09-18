/*
 * Server side of game communication.
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

#ifndef PUTKARTS_Connection_Server_HPP
#define PUTKARTS_Connection_Server_HPP

#include <string>
#include <set>
#include <map>
#include <boost/enable_shared_from_this.hpp>
#include <boost/utility.hpp>

#include "connection/Base.hpp"
#include "connection/EndPoint.hpp"
#include "util/Clock.hpp"

namespace Connection {
	class Server;
	class Client;
}

/**
 * Base class for game servers.
 */
class Connection::Server: virtual public Connection::Base, public boost::enable_shared_from_this<Connection::Server> {
public:
	/** Interface for listeners. */
	class Listener: boost::noncopyable {
	public:
		/**
		 * Virtual destructor.
		 */
		virtual ~Listener() {
			// Nothing to do.
		}

		/**
		 * Inform the server about changes.
		 *
		 * @param server The server.
		 * @return false if this Listener should be removed, true otherwise.
		 */
		virtual bool update(Server& server) = 0;
	};

private:
	/** Server side class for handling clients. */
	class Client;

	/** Class for local clients. */
	class LocalClient;

	/** Type for listener container. Use std::map for consistent order. */
	typedef std::set<boost::shared_ptr<Listener> > ListenerContainerType;

	/** Type for client container. Use std::map for consistent order. */
	typedef std::map<int, boost::shared_ptr<Client> > ClientContainerType;

	/** Listeners that wait for connections. */
	ListenerContainerType listeners;

	/** List of connected clients. */
	ClientContainerType clients;

	/** The clock used for timing the game. */
	Clock clock;

	/**
	 * Insert a new client.
	 *
	 * @param client The client.
	 */
	void addClient(boost::shared_ptr<Client> client);

	/**
	 * Send a packet to all clients.
	 *
	 * @param data The data.
	 */
	void sendPacket(const std::string& data);

	/**
	 * Send a message to all clients.
	 *
	 * @param msg The message.
	 */
	void sendMessage(const Game::Message& msg);

public:
	/**
	 * Create a local client.
	 */
	boost::shared_ptr<Connection::Client> createLocalClient();

	/**
	 * Insert a new client.
	 *
	 * @param connection The channel of communication.
	 */
	void addClient(boost::shared_ptr<EndPoint> connection);

	/**
	 * Insert a new listener.
	 *
	 * @param listener The listener.
	 */
	void addListener(boost::shared_ptr<Listener> listener);

	/**
	 * Handle data from the clients, and update the game state.
	 */
	virtual void update();

	/**
	 * Start the clock.
	 */
	virtual void startGame();
};

#endif

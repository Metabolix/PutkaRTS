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

#include "boost/weak_ptr.hpp"
#include "boost/bind.hpp"

#include "Server.hpp"
#include "Client.hpp"
#include "connection/EndPoint.hpp"
#include "connection/PipePair.hpp"
#include "connection/ClientInfo.hpp"
#include "game/Game.hpp"

/**
 * A class that represents one client on the server side.
 */
class Connection::Server::Client: public Connection::ClientInfo {
	/** The server class is a friend. */
	friend class Server;

	/** Connection to the client. */
	boost::shared_ptr<EndPoint> connection;

public:
	/**
	 * Construct a new client that lives behind the given connection.
	 *
	 * @param conn Connection to the client.
	 */
	Client(boost::shared_ptr<EndPoint> conn):
		connection(conn) {
	}

	/**
	 * Destructor.
	 */
	virtual ~Client() {
	}
};

/**
 * A class that's used for local clients on the client side.
 */
class Connection::Server::LocalClient: public Connection::Client {
	/** Shared pointer to the server. LocalClient keeps it alive. */
	boost::shared_ptr<Server> server;

public:
	/**
	 * Constructor.
	 *
	 * @param server_ The server.
	 * @param endPoint The connection (one end of a pipe).
	 */
	LocalClient(boost::shared_ptr<Server> server_, boost::shared_ptr<EndPoint> endPoint):
		Connection::Client(endPoint),
		server(server_) {
	}

	/** @copydoc Client::setReadyToInit */
	void setReadyToInit() {
		if (!server->game.get()) {
			server->initGame();
			Base::initGame();
		}
	}

	/** @copydoc Client::setReadyToStart */
	void setReadyToStart() {
		if (server->game.get()) {
			server->startGame();
		}
	}

	/**
	 * Update both the server and the client.
	 *
	 * @see Server::update
	 * @see Client::update
	 */
	void update() {
		server->update();
		Client::update();
	}
};

boost::shared_ptr<Connection::Client> Connection::Server::createLocalClient() {
	PipePair p;
	addClient(p.getEnd1());
	return boost::shared_ptr<Connection::Client>(new LocalClient(shared_from_this(), p.getEnd2()));
}

void Connection::Server::startGame() {
	clock.reset();
	clock.unpause();
}

void Connection::Server::addClient(boost::shared_ptr<Client> client) {
	for (client->id = 1; clients.find(client->id) != clients.end(); ++client->id);
	clients[client->id] = client;
}

void Connection::Server::addClient(boost::shared_ptr<EndPoint> connection) {
	return addClient(boost::shared_ptr<Client>(new Client(connection)));
}

void Connection::Server::update() {
	for (ClientContainerType::iterator i = clients.begin(); i != clients.end(); ++i) {
		Client& client = *i->second;
		EndPoint& endPoint = *client.connection;
		std::string data;
		while (endPoint.receivePacket(data)) {
			game->insertMessage(Game::Message(data));
		}
	}
	game->runUntil(clock.getTime(), boost::bind(&Server::sendMessage, this, _1));

	// PING
	Game::Message msg;
	msg.timestamp = game->getTime();
	sendMessage(msg);
}

void Connection::Server::sendPacket(const std::string& data) {
	for (ClientContainerType::iterator i = clients.begin(); i != clients.end(); ++i) {
		i->second->connection->sendPacket(data);
	}
}

void Connection::Server::sendMessage(const Game::Message& msg) {
	sendPacket(msg.serialize());
}

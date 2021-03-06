#include <memory>
#include <mutex>
#include <thread>
#include <functional>
#include <string>

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
	std::shared_ptr<EndPoint> connection;

public:
	/**
	 * Construct a new client that lives behind the given connection.
	 *
	 * @param conn Connection to the client.
	 */
	Client(std::shared_ptr<EndPoint> conn):
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
	std::shared_ptr<Server> server;

public:
	/**
	 * Constructor.
	 *
	 * @param server_ The server.
	 * @param endPoint The connection (one end of a pipe).
	 */
	LocalClient(std::shared_ptr<Server> server_, std::shared_ptr<EndPoint> endPoint):
		Connection::Client(endPoint),
		server(server_) {
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

void Connection::Server::run() {
	std::weak_ptr<Server> weak(shared_from_this());
	while (std::shared_ptr<Server> ptr = weak.lock()) {
		if (state == END) {
			return;
		}
		update();
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}
}

std::shared_ptr<Connection::Client> Connection::Server::createLocalClient() {
	PipePair p;
	addClient(p.getEnd1());
	return std::shared_ptr<Connection::Client>(new LocalClient(shared_from_this(), p.getEnd2()));
}

void Connection::Server::startGame() {
	Base::startGame();
	clock.reset();
	clock.unpause();
}

void Connection::Server::addClient(std::shared_ptr<Client> client) {
	std::lock_guard<std::recursive_mutex> lock(*this);
	for (client->id = 1; clients.find(client->id) != clients.end(); ++client->id);

	// Send the new client to all, and send all old clients to the new one.
	ClientInfoContainerType old = clients;
	clients[client->id] = client;
	sendPacket(clients, 'c' + client->serialize());
	for (ClientInfoContainerType::iterator i = old.begin(); i != old.end(); ++i) {
		sendPacket(*client, 'c' + i->second->serialize());
	}
}

void Connection::Server::addClient(std::shared_ptr<EndPoint> connection) {
	return addClient(std::shared_ptr<Client>(new Client(connection)));
}

void Connection::Server::removeClient(int id) {
	std::lock_guard<std::recursive_mutex> lock(*this);
	if (clients.erase(id)) {
		sendPacket(clients, 'd' + std::to_string(id));
	}
}

void Connection::Server::addListener(std::shared_ptr<Listener> listener) {
	std::lock_guard<std::recursive_mutex> lock(*this);
	listeners.insert(listener);
}

bool Connection::Server::handlePacket(Client& client, std::string& data) {
	char type = *data.begin();
	data.erase(data.begin());

	// Game::Message.
	if (type == 'm') {
		if (game) {
			Game::Message msg(data);
			msg.client = client.id;
			game->insertMessage(msg);
		}
		return true;
	}

	// Ready to init.
	if (type == 'i') {
		if (client.readyToInit) {
			return true;
		}
		client.readyToInit = true;
		bool readyToInit = true;
		for (ClientInfoContainerType::iterator i = clients.begin(); readyToInit && i != clients.end(); ++i) {
			readyToInit &= i->second->readyToInit;
		}
		if (readyToInit) {
			sendPacket(clients, "i");
			initGame();
			listeners.clear();
		}
		return true;
	}

	// Ready to start.
	if (type == 's') {
		if (client.readyToStart) {
			return true;
		}
		client.readyToStart = true;
		bool readyToStart = true;
		for (ClientInfoContainerType::iterator i = clients.begin(); readyToStart && i != clients.end(); ++i) {
			readyToStart &= i->second->readyToStart;
		}
		if (readyToStart) {
			sendPacket(clients, "s");
			startGame();
		}
		return true;
	}

	// Invalid packet.
	return false;
}

void Connection::Server::update() {
	std::lock_guard<std::recursive_mutex> lock(*this);
	if (state != SETUP) {
		if (clients.empty()) {
			state = END;
			return;
		}
	}
	if (state == SETUP && !listeners.empty()) {
		try {
			metaserver.sendGame(*this);
		} catch (...) {
			// FIXME: Something went wrong, log it and tell the user!
		}
	}
	for (ListenerContainerType::iterator i = listeners.begin(); i != listeners.end();) {
		ListenerContainerType::iterator j = i++;
		Listener& listener = **j;
		if (!listener.update(*this)) {
			listeners.erase(j);
		}
	}
	for (ClientInfoContainerType::iterator i = clients.begin(); i != clients.end();) {
		ClientInfoContainerType::iterator j = i++;
		Client& client = dynamic_cast<Client&>(*j->second);
		EndPoint& endPoint = *client.connection;
		std::string data;
		while (true) {
			try {
				if (!endPoint.receivePacket(data)) {
					break;
				}
			} catch (...) {
				removeClient(j->first);
				break;
			}
			if (!data.empty()) {
				if (!handlePacket(client, data)) {
					removeClient(j->first);
					break;
				}
			}
		}
	}
	if (state == PLAY) {
		game->runUntil(clock.getTime(), std::bind(&Server::sendMessage, this, std::placeholders::_1));

		// PING
		Game::Message msg;
		msg.timestamp = game->getTime();
		sendMessage(msg);
	}
}

void Connection::Server::sendPacket(Client& client, const std::string& data) {
	try {
		client.connection->sendPacket(data);
	} catch (...) {
		removeClient(client.id);
	}
}

void Connection::Server::sendPacket(const ClientInfoContainerType& clients, const std::string& data) {
	for (ClientInfoContainerType::const_iterator i = clients.begin(); i != clients.end();) {
		// Handle the iterator carefully, clients may be erased.
		ClientInfoContainerType::const_iterator j = i++;
		sendPacket(dynamic_cast<Client&>(*j->second), data);
	}
}

void Connection::Server::sendMessage(const Game::Message& msg) {
	sendPacket(clients, 'm' + msg.serialize());
}

void Connection::Server::setName(const std::string& name_) {
	name = name_;
}

std::string Connection::Server::getName() const {
	return name;
}

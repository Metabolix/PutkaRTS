#include <memory>
#include <boost/lexical_cast.hpp>

#include "Client.hpp"

#include "game/Game.hpp"

void Connection::Client::handlePacket(std::string& data) {
	char type = *data.begin();
	data.erase(data.begin());

	// A client has joined.
	if (type == 'c') {
		ClientInfo info(data);
		clients[info.id] = std::make_shared<ClientInfo>(info);
		if (clients.size() == 1) {
			ownId = info.id;
		}
		return;
	}

	// A client has left.
	if (type == 'd') {
		clients.erase(boost::lexical_cast<int>(data));
		return;
	}

	// Game::Message.
	if (type == 'm') {
		if (game) {
			Game::Message msg(data);
			if (msg.timestamp > lastMessageTimestamp) {
				prevMessageTimestamp = lastMessageTimestamp;
				lastMessageTimestamp = msg.timestamp;
			}
			game->insertMessage(msg);
		}
		return;
	}

	// Init the game.
	if (type == 'i') {
		initGame();
		return;
	}

	// Start the game.
	if (type == 's') {
		startGame();
		return;
	}
}

void Connection::Client::update() {
	std::string data;
	while (true) {
		try {
			if (!connection->receivePacket(data)) {
				break;
			}
		} catch (...) {
			throw std::runtime_error("Disconnected unexpectedly!");
			break;
		}
		if (!data.empty()) {
			handlePacket(data);
		}
	}
	if (state == PLAY) {
		game->runUntil(prevMessageTimestamp);
	}
}

void Connection::Client::sendMessage(const Game::Message& message) {
	connection->sendPacket("m" + message.serialize());
}

void Connection::Client::setReadyToInit() {
	connection->sendPacket("i");
}

void Connection::Client::setReadyToStart() {
	connection->sendPacket("s");
}

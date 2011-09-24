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

#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>

#include "Client.hpp"

#include "game/Game.hpp"

boost::shared_ptr<const Game::Player> Connection::Client::getPlayer() const {
	// TODO: Implement properly!
	return game->getPlayers().begin()->second;
}

void Connection::Client::handlePacket(std::string& data) {
	char type = *data.begin();
	data.erase(data.begin());
	if (type == 'c') {
		ClientInfo info(data);
		clients[info.id] = boost::make_shared<ClientInfo>(info);
		if (clients.size() == 1) {
			ownId = info.id;
		}
		return;
	}
	if (type == 'd') {
		clients.erase(boost::lexical_cast<int>(data));
		return;
	}
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
	if (type == 'i') {
		initGame();
		return;
	}
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

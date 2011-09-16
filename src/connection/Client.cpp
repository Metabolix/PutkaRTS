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

#include "Client.hpp"

#include "game/Game.hpp"

boost::shared_ptr<const Game::Player> Connection::Client::getPlayer() const {
	// TODO: Implement properly!
	return game->getPlayers().begin()->second;
}

void Connection::Client::runUntilNow() {
	Game::Message msg;
	std::string data;
	while (connection->receivePacket(data)) {
		msg = Game::Message(data);
		game->insertMessage(msg);
	}
	game->runUntil(msg.timestamp);
}

void Connection::Client::sendMessage(const Game::Message& message) {
	connection->sendPacket(message.serialize());
}

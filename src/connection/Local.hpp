/*
 * Local game dummy communication.
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

#ifndef PUTKARTS_Connection_Local_HPP
#define PUTKARTS_Connection_Local_HPP

#include <string>

#include "game/Message.hpp"
#include "connection/Server.hpp"
#include "connection/Client.hpp"

namespace Connection {
	class Local;
}

/**
 * Local game.
 */
class Connection::Local: virtual public Connection::Server, virtual public Connection::Client {
public:
	/**
	 * Send a message describing a player action.
	 *
	 * @param message The message to send.
	 */
	void sendMessage(const Game::Message& message) {
		game->insertMessage(message);
	}

	/**
	 * @see Client::setReadyToInit
	 */
	void setReadyToInit() {
		if (!game.get()) {
			initGame();
		}
	}

	/**
	 * @see Client::setReadyToStart
	 */
	void setReadyToStart() {
		if (game.get()) {
			startGame();
		}
	}
};

#endif

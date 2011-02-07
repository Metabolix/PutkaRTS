/*
 * Class for remote game connection.
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

#ifndef _REMOTEGAMECONNECTION_HPP
#define _REMOTEGAMECONNECTION_HPP

#include <string>

#include "game/Message.hpp"
#include "GameConnection.hpp"

/**
 * Class for participating in remote games.
 */
class RemoteGameConnection: public GameConnection {
	sf::Clock clock;
public:
	/**
	 * Constructor.
	 */
	RemoteGameConnection(std::auto_ptr<Game> game_):
		GameConnection(game_) {
	}

	/**
	 * Send a message describing a client action.
	 *
	 * @param message The message to send.
	 */
	void sendMessage(const Message& message) {
		// TODO: send the message to the server!
	}

	/**
	 * Run the game up to this moment; this includes reading messages from the server.
	 */
	void runUntilNow() {
		Message message;
		// TODO: read messages from somewhere and feed them to Game::instance->addMessage(message)!
		// TODO: do not run until message.time, that won't be smooth!
		game->runUntil(message.timestamp);
	}
};

#endif

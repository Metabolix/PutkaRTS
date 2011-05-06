/*
 * Class for local game "connection".
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

#ifndef PUTKARTS_LocalGameConnection_HPP
#define PUTKARTS_LocalGameConnection_HPP

#include <string>

#include "Message.hpp"
#include "GameConnection.hpp"

/**
 * Class for running local games.
 */
class LocalGameConnection: public GameConnection {
	/** The local clock. */
	sf::Clock clock;
public:
	/**
	 * Constructor.
	 */
	LocalGameConnection(boost::shared_ptr<Game> game_):
		GameConnection(game_) {
	}

	/**
	 * Send a message describing a player action.
	 *
	 * @param message The message to send; this implementation passes it straight to the current Game.
	 */
	void sendMessage(const Message& message) {
		game->insertMessage(message);
	}

	/**
	 * Run the game up to this moment.
	 */
	void runUntilNow() {
		game->runUntil(clock.GetElapsedTime());
	}
};

#endif

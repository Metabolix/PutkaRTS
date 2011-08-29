/*
 * Base class for game connections.
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

#ifndef PUTKARTS_GameConnection_HPP
#define PUTKARTS_GameConnection_HPP

#include <string>
#include <stdexcept>
#include <boost/shared_ptr.hpp>

#include "game/Game.hpp"
#include "game/Message.hpp"

/**
 * Base class for local and remote game connections.
 */
class GameConnection {
protected:
	/** The current game. */
	boost::shared_ptr<Game::Game> game;
public:
	/**
	 * Constructor.
	 *
	 * @param game_ The current game instance.
	 */
	GameConnection(boost::shared_ptr<Game::Game> game_): game(game_) {
		if (!game.get()) {
			throw std::logic_error("GameConnection: game is NULL!");
		}
	}

	/**
	 * Virtual base destructor.
	 */
	virtual ~GameConnection() {
		// Nothing to do.
	}

	/**
	 * Get the game object.
	 */
	const Game::Game& getGame() const {
		return *game;
	}

	/**
	 * Send a message describing a player action.
	 *
	 * @param message The message to send.
	 */
	virtual void sendMessage(const Game::Message& message) = 0;

	/**
	 * Run the game up to this moment.
	 */
	virtual void runUntilNow() = 0;
};

#endif

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

#ifndef PUTKARTS_Connection_Base_HPP
#define PUTKARTS_Connection_Base_HPP

#include <string>
#include <stdexcept>
#include <boost/shared_ptr.hpp>

namespace Connection {
	class Base;

	/** The current state. */
	enum State {
		SETUP, INIT, PLAY, END
	};
}

namespace Game {
	class Game;
	class Message;
}

/**
 * Base class for local and remote game connections.
 */
class Connection::Base {
protected:
	/** The current state. */
	State state;

	/** The current game. */
	boost::shared_ptr<Game::Game> game;

	/**
	 * Initialise the game object.
	 */
	virtual void initGame();

	/**
	 * Start the game.
	 */
	virtual void startGame();

public:
	/**
	 * Constructor.
	 */
	Base():
		state(SETUP) {
	}

	/**
	 * Virtual base destructor.
	 */
	virtual ~Base() {
		// Nothing to do.
	}

	/**
	 * Get the current state.
	 *
	 * @return The current state.
	 */
	State getState() const {
		return state;
	}

	/**
	 * Get the game object.
	 */
	const Game::Game& getGame() const {
		if (!game.get()) {
			throw std::logic_error("Connection::Base: game is NULL!");
		}
		return *game;
	}

	/**
	 * Run the game up to this moment.
	 */
	virtual void update() = 0;
};

#endif

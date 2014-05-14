#ifndef PUTKARTS_Connection_Base_HPP
#define PUTKARTS_Connection_Base_HPP

#include <string>
#include <stdexcept>
#include <map>
#include <memory>

#include "connection/ClientInfo.hpp"

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
	/** Type for client container. Use std::map for consistent order. */
	typedef std::map<int, std::shared_ptr<ClientInfo> > ClientInfoContainerType;

	/** List of clients (players) in this session. */
	ClientInfoContainerType clients;

	/** The current state. */
	State state;

	/** The current game. */
	std::shared_ptr<Game::Game> game;

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

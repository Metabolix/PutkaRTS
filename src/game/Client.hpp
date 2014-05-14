#ifndef PUTKARTS_Game_Client_HPP
#define PUTKARTS_Game_Client_HPP

#include <string>
#include <unordered_set>

#include "game/Player.hpp"

namespace Game {
	class Client;
}

/**
 * A source of commands in the game.
 *
 * Each person playing the game has one instance of Game::Client. As such, the
 * clients are not managed in Game::Game but in Connection::Base instead.
 */
struct Game::Client {
	/** Type for client's id */
	typedef int IdType;

	/** The client id set by the server. */
	IdType id;

	/** The name of this client. */
	std::string name;

	/** Is this a computer player? */
	bool ai;

	/** What players does this client command? */
	std::unordered_set<Player::IdType> players;

	/** Constructor; sets the default values. */
	Client():
		id(0),
		name("Unknown"),
		ai(false) {
	}

	/**
	 * Virtual destructor.
	 */
	virtual ~Client() {}
};

#endif

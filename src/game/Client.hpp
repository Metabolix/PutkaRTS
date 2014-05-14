/*
 * Client class definition.
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

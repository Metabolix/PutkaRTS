/*
 * Player class definition.
 *
 * Copyright 2011 Mika Katajamäki
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

#ifndef PUTKARTS_Player_HPP
#define PUTKARTS_Player_HPP

#include <string>

namespace Game {
	class Player;
}

class Game::Player {
public:
	/** Type for player's id */
	typedef int IdType;

	/** Id of the player */
	IdType id;

private:
	/** Name of the player */
	std::string name;

public:
	/**
	 * Constructor.
	 *
	 * @param name_ Name of the player.
	 */
	Player(const std::string &name_) :
		id(0),
		name(name_) {
	};

	/**
	 * Get the name of the player.
	 *
	 * @return Returns reference to the string containing player's name.
	 */
	const std::string& getName() const {
		return name;
	}
};

#endif
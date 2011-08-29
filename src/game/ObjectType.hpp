/*
 * Game object type class definition.
 *
 * Copyright 2011 Petri Österman
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

#ifndef PUTKARTS_Game_ObjectType_HPP
#define PUTKARTS_Game_ObjectType_HPP

#include <string>
#include <iostream>

namespace Game {
	class ObjectType;
}

/**
 * This class describes object type.
 */
class Game::ObjectType {
	/** Object type's friends */
	friend class ObjectAction;

	/** Object type's animation set name. */
	std::string animationSet;

	/** Object type's name. */
	std::string name;

	/** Object type's maximum hit points. */
	int maxHitPoints;

public:
	/**
	 * Constructor that loads the object type data from stream.
	 *
	 * @param stream Stream to read object type data.
	 */
	ObjectType(std::istream& stream);

	/**
	 * Return object type's maximum hit points.
	 *
	 * @return Object type's maximum hit points.
	 */
	int getMaxHitPoints() const {
		return maxHitPoints;
	}
};

#endif

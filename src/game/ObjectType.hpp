/*
 * Game object type class definition.
 *
 * Copyright 2011 Petri Österman
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

#ifndef PUTKARTS_Game_ObjectType_HPP
#define PUTKARTS_Game_ObjectType_HPP

#include "util/Scalar.hpp"
#include <string>
#include <iostream>

namespace Game {
	class ObjectType;
}

/**
 * This class describes object type.
 */
struct Game::ObjectType {
	typedef std::string IdType;

	/** An unique identifier for this type. */
	IdType id;

	/** The name to tell the users. */
	std::string name;

	/** Does this kind of unit ever change? */
	bool immutable;

	/** The size. */
	Scalar<SIUnit::Length> radius;

	/** The maximum velocity. */
	Scalar<SIUnit::Velocity> maxVelocity;

	/** The line of sight. */
	Scalar<SIUnit::Length> lineOfSight;

	/** Maximum hit points. */
	int maxHitPoints;
};

#endif

/*
 * Object action class definition.
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

#ifndef PUTKARTS_Game_ObjectAction_HPP
#define PUTKARTS_Game_ObjectAction_HPP

#include <string>
#include <iostream>
#include <boost/shared_ptr.hpp>

namespace Game {
	class Game;
	class Object;
	class ObjectAction;
}

/**
 * This class describes object action.
 */
class Game::ObjectAction {
public:
	typedef std::string IdType;

	/** Predefined action "new". */
	static const IdType NEW;

	/** Predefined action "delete". */
	static const IdType DELETE;

	/** Predefined action "move". */
	static const IdType MOVE;

	/** An unique identifier for this action. */
	const IdType id;

private:
	/** The name to tell the users. */
	std::string name;

public:
	/**
	 * Constructor.
	 */
	ObjectAction();

	/**
	 * Run object action.
	 *
	 * @param actor Acting object.
	 * @param target Target of the action.
	 * @return Whether it was possible to run the action.
	 */
	bool runAction(boost::shared_ptr<Object> actor, boost::shared_ptr<Object> target);
};

#endif

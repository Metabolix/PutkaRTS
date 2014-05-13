/*
 * Task class definition.
 *
 * Copyright 2012 Lauri Kenttä
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

#ifndef PUTKARTS_Game_Task_HPP
#define PUTKARTS_Game_Task_HPP

#include <list>
#include <memory>

namespace Game {
	class Task;
	class Object;
	class ObjectAction;
}

/**
 * This class describes a task that some units are performing.
 */
struct Game::Task {
	/** The action. */
	std::shared_ptr<const ObjectAction> action;

	/** The actor objects. */
	std::list<std::weak_ptr<Object> > actors;

	/** The target objects. */
	std::list<std::weak_ptr<Object> > targets;

	/** A dummy target. This is used if there aren't any real targets, e.g. when moving to a location. */
	std::shared_ptr<Object> dummy;
};

#endif

/*
 * Game object class definition.
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

#ifndef PUTKARTS_Game_Object_HPP
#define PUTKARTS_Game_Object_HPP

#include <memory>

#include "util/Vector2.hpp"
#include "ObjectType.hpp"
#include "Player.hpp"

namespace Game {
	class Game;
	class Message;
	class Task;
	class Object;
	class ObjectAction;
}

/**
 * This class describes world object.
 */
class Game::Object {
public:
	friend class Game;
	friend class ObjectAction;
	typedef unsigned int IdType;

	/** Id of the object. */
	IdType id;

private:
	/** Pointer to object's type. */
	std::shared_ptr<const ObjectType> objectType;

	/** Object's owner. */
	std::shared_ptr<const Player> owner;

	/** Set if the object is dead (i.e. ready to be removed from the game). */
	bool dead;

	/** Object's position. */
	Vector2<SIUnit::Position> position;

	/** Direction the object is looking. */
	Scalar<SIUnit::Angle> direction;

	/** The current task of the object. */
	std::shared_ptr<Task> task;

	/** Object's hit points. */
	int hitPoints;

	/** Object's experience. */
	int experience;

public:
	/**
	 * Constructor.
	 *
	 * @param position Object's starting position.
	 */
	Object(const Vector2<SIUnit::Position>& position);

	/**
	 * Return object's Object type.
	 *
	 * @return Object's object type.
	 */
	const std::shared_ptr<const ObjectType> getObjectType() const {
		return objectType;
	}

	/**
	 * Return object's position.
	 *
	 * @return Position of the object.
	 */
	Vector2<SIUnit::Position> getPosition() const {
		return position;
	}

	/**
	 * Return the direction the object is looking.
	 *
	 * @return Direction of the object.
	 */
	Scalar<SIUnit::Angle> getDirection() const {
		return direction;
	}

	/**
	 * Return object's hit points.
	 *
	 * @return Object's hit points.
	 */
	int getHitPoints() const {
		return hitPoints;
	}

	/**
	 * Return object's experience.
	 *
	 * @return Object's experience.
	 */
	int getExperience() const {
		return experience;
	}

	/**
	 * Checks if object is within the given range of the given position.
	 *
	 * @param pos The position
	 * @param range The range
	 * @return True if object is within the given range of the given point.
	 */
	bool isNear(const Vector2<SIUnit::Position> &pos, const Scalar<SIUnit::Length> &range = Scalar<SIUnit::Length>(0)) const {
		return (position - pos).pow2() < (objectType->radius + range).pow2();
	}

	/**
	 * Get the owner of this object.
	 *
	 * @return Returns a pointer to the owner of this object.
	 */
	const std::shared_ptr<const Player> getOwner() const {
		return owner;
	}

	/**
	 * Move the unit one step in time.
	 *
	 * @param dt The time step size.
	 * @param game The game
	 * @return true if the object is still alive after the step.
	 */
	bool runStep(Scalar<SIUnit::Time> dt, Game& game);
protected:
	/**
	 * Set object's position.
	 *
	 * @param position_ Position to set.
	 */
	void setPosition(const Vector2<SIUnit::Position>& position_) {
		position = position_;
	}

	/**
	 * Set object's direction.
	 *
	 * @param direction_ Direction to set.
	 */
	void setDirection(const Scalar<SIUnit::Angle>& direction_) {
		direction = direction_;
	}

	/**
	 * Set object's hit points.
	 *
	 * @param hitPoints Hit points to set.
	 */
	void setHitPoints(int hitPoints);

	/**
	 * Set object's experience.
	 *
	 * @param experience Experience to set.
	 */
	void setExperience(int experience);

	/**
	 * Add (or subtract) object's hit points.
	 *
	 * @param change Hit points change.
	 */
	void addHitPoints(int change) {
		setHitPoints(getHitPoints() + change);
	}

	/**
	 * Add (or subtract) object's experience.
	 *
	 * @param change Experience change.
	 */
	void addExperience(int change) {
		setExperience(getExperience() + change);
	}
};

#endif

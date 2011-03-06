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

#ifndef PUTKARTS_World_Object_HPP
#define PUTKARTS_World_Object_HPP

#include "util/Vector2.hpp"
#include "ObjectType.hpp"

#include <boost/shared_ptr.hpp>

namespace World {
	/**
	 * This class describes world object.
	 */
	class Object {
		friend class ObjectAction;

		/** Pointer to object's type. */
		boost::shared_ptr<const ObjectType> objectType;

		/** Object's position. */
		Vector2<SIUnit::Position> position;

		/** Direction the object is looking. */
		Scalar<SIUnit::Angle> direction;

		/** Object's hit points. */
		int hitPoints;

		/** Object's experience. */
		int experience;

	public:
		/**
		 * Constructor.
		 *
		 * @param objectType Object's type.
		 * @param position Object's starting position.
		 */
		Object(const boost::shared_ptr<const ObjectType> objectType, const Vector2<SIUnit::Position>& position);

		/**
		 * Return object's Object type.
		 *
		 * @return Object's object type.
		 */
		const boost::shared_ptr<const ObjectType> getObjectType() const {
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
}

#endif

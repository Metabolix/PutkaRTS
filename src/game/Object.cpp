/*
 * Game object class implementation.
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

#include "Object.hpp"
#include "Message.hpp"

Game::Object::Object(const boost::shared_ptr<const ObjectType> objectType_, boost::shared_ptr<const Player> owner_, const Vector2<SIUnit::Position>& position_):
	id(0),
	objectType(objectType_),
	owner(owner_),
	position(position_),
	direction(0),
	targetPosition(position_),
	hitPoints(objectType_->maxHitPoints),
	experience(0) {
}

void Game::Object::setHitPoints(int hitPoints_) {
	hitPoints = hitPoints_;
	if (hitPoints > objectType->maxHitPoints) {
		hitPoints = objectType->maxHitPoints;
	} else if (hitPoints < 0) {
		hitPoints = 0;
	}
}

void Game::Object::setExperience(int experience_) {
	experience = experience_;
	if (experience < 0) {
		experience = 0;
	}
}

bool Game::Object::runStep(Scalar<SIUnit::Time> dt, Game& game) {
	// TODO: Handle whatever the object is doing.
	// TODO: Check collisions before moving!
	if (position != targetPosition) {
		Vector2<SIUnit::Position> old = position;
		direction = (targetPosition - position).toAngle();
		position += Vector2<>::fromAngle(direction) * objectType->maxVelocity * dt;
		if ((targetPosition - old).dot(targetPosition - position).isNegative()) {
			position = targetPosition;
		}
	}
	return true;
}

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

#include <boost/foreach.hpp>

#include "Object.hpp"
#include "ObjectAction.hpp"
#include "Task.hpp"
#include "Game.hpp"

Game::Object::Object(const Vector2<SIUnit::Position>& position_):
	id(0),
	dead(false),
	position(position_),
	direction(0),
	hitPoints(0),
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
	if (dead) {
		return false;
	}
	if (!task) {
		return true;
	}

	// Find the closest target.
	std::shared_ptr<Object> target;
	BOOST_FOREACH(std::weak_ptr<Object> objectWeak, task->targets) {
		std::shared_ptr<Object> object(objectWeak.lock());
		if (!object || object->dead) {
			continue;
		}
		if (!target || (position - object->position).pow2() < (position - target->position).pow2()) {
			target = object;
		}
	}

	// If no target is found, the task is finished.
	if (!target) {
		task.reset();
		return true;
	}

	// TODO: Handle whatever the object is doing.
	// TODO: Check collisions before moving!
	if (position != target->position) {
		Vector2<SIUnit::Position> old = position;
		direction = (target->position - position).toAngle();
		position += Vector2<>::fromAngle(direction) * objectType->maxVelocity * dt;
		if ((target->position - old).dot(target->position - position).isNegative()) {
			position = target->position;
		}
	}
	return true;
}

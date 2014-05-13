/*
 * GUI::GameObject class implementation.
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

#include "game/Client.hpp"
#include "game/Object.hpp"

#include "gui/game/Object.hpp"

GUI::Game::Object::Object(std::shared_ptr<const ::Game::Object> object_):
	object(object_) {
}

void GUI::Game::Object::draw(sf::RenderWindow& window, std::shared_ptr<const ::Game::Client> viewer, bool selected) {
	// TODO: Load real graphics and track animations.
	sf::ConvexShape arrow;
	sf::CircleShape circle;
	arrow.setPointCount(4);
	arrow.setPoint(0, sf::Vector2f(+0.9, -0.0));
	arrow.setPoint(1, sf::Vector2f(-0.4, -0.6));
	arrow.setPoint(2, sf::Vector2f(-0.7, -0.0));
	arrow.setPoint(3, sf::Vector2f(-0.4, +0.6));
	arrow.setOutlineColor(sf::Color(0, 0, 0, 0xff));
	arrow.setOutlineThickness(0.15);
	circle.setPointCount(12);
	circle.setRadius(1);
	circle.setOrigin(1, 1);
	circle.setFillColor(sf::Color(0xff, 0xff, 0xff, 0x44));
	circle.setOutlineThickness(0.2);
	circle.setOutlineColor(sf::Color(0xff, 0xff, 0xff, 0xdd));

	Vector2<SIUnit::Position> pos = object->getPosition();
	double r = object->getObjectType()->radius.getDouble();

	if (viewer->players.find(object->getOwner()->id) != viewer->players.end()) {
		circle.setFillColor(sf::Color(0x33, 0x33, 0xcc));
		arrow.setFillColor(sf::Color(0x33, 0x33, 0xcc));
	} else {
		circle.setFillColor(sf::Color(0xcc, 0x33, 0x00));
		arrow.setFillColor(sf::Color(0xdd, 0x00, 0x00));
	}

	if (selected) {
		circle.setScale(r, r);
		circle.setPosition(pos.x.getDouble(), pos.y.getDouble());
		window.draw(circle);
	}

	arrow.setScale(r, r);
	arrow.setPosition(pos.x.getDouble(), pos.y.getDouble());
	arrow.setRotation(Math::toDegrees(object->getDirection().getDouble()));
	window.draw(arrow);
}

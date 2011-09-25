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

GUI::Game::Object::Object(boost::shared_ptr<const ::Game::Object> object_):
	object(object_) {
}

void GUI::Game::Object::draw(sf::RenderWindow& window, boost::shared_ptr<const ::Game::Client> viewer, bool selected) {
	// TODO: Load real graphics and track animations.
	static sf::Shape tmp, circle;
	if (!tmp.GetNbPoints()) {
		tmp.AddPoint(-0.8, -0.4);
		tmp.AddPoint(-0.0, -0.4);
		tmp.AddPoint(-0.0, -0.8);
		tmp.AddPoint(+0.8, -0.0);
		tmp.AddPoint(-0.0, +0.8);
		tmp.AddPoint(-0.0, +0.4);
		tmp.AddPoint(-0.8, +0.4);
		tmp.SetOutlineWidth(0.15);
	}
	if (!circle.GetNbPoints()) {
		circle = sf::Shape::Circle(0, 0, 1, sf::Color(0xff, 0xff, 0xff, 0x44), 0.2, sf::Color(0xff, 0xff, 0xff, 0xdd));
	}

	Vector2<SIUnit::Position> pos = object->getPosition();
	double r = object->getObjectType()->radius.getDouble();

	if (viewer->players.find(object->getOwner()->id) != viewer->players.end()) {
		circle.SetColor(sf::Color(0x33, 0x33, 0xcc));
		tmp.SetColor(sf::Color(0x33, 0x33, 0xcc));
	} else {
		circle.SetColor(sf::Color(0xcc, 0x33, 0x00));
		tmp.SetColor(sf::Color(0xdd, 0x00, 0x00));
	}

	if (selected) {
		circle.SetScale(r, r);
		circle.SetPosition(pos.x.getDouble(), pos.y.getDouble());
		window.Draw(circle);
	}

	tmp.SetScale(r, r);
	tmp.SetPosition(pos.x.getDouble(), pos.y.getDouble());
	tmp.SetRotation(-Math::toDegrees(object->getDirection().getDouble()));
	window.Draw(tmp);
}

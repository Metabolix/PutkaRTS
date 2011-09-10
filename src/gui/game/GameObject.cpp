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

#include "GameObject.hpp"

GUI::GameObject::GameObject(boost::shared_ptr<const Game::Object> object_):
	object(object_) {
}

void GUI::GameObject::draw(sf::RenderWindow& window, bool selected) {
	// TODO: Load real graphics and track animations.
	static sf::Shape tmp;
	if (!tmp.GetNbPoints()) {
		tmp.AddPoint(-0.8, -0.4);
		tmp.AddPoint(-0.0, -0.4);
		tmp.AddPoint(-0.0, -0.8);
		tmp.AddPoint(+0.8, -0.0);
		tmp.AddPoint(-0.0, +0.8);
		tmp.AddPoint(-0.0, +0.4);
		tmp.AddPoint(-0.8, +0.4);
	}
	Vector2<SIUnit::Position> pos = object->getPosition();
	double r = object->getObjectType()->radius.getDouble();
	if (selected) {
		window.Draw(sf::Shape::Circle(pos.x.getDouble(), pos.y.getDouble(), r, sf::Color(128, 128, 255, 96), 0.1f, sf::Color(192, 192, 255)));
	}
	tmp.SetScale(r, r);
	tmp.SetPosition(pos.x.getDouble(), pos.y.getDouble());
	tmp.SetRotation(-Math::toDegrees(object->getDirection().getDouble()));
	window.Draw(tmp);
}

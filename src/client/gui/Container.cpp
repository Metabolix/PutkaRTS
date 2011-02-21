/*
 * GUI Container class implementation.
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

#include "Container.hpp"

#include <algorithm>
#include <boost/bind.hpp>

void GUI::Container::insert(boost::shared_ptr<Object> object) {
	objects.push_back(object);
}

void GUI::Container::erase(boost::shared_ptr<Object> object) {
	ObjectListType::iterator i = std::find(objects.begin(), objects.end(), object);
	if (i != objects.end()) {
		objects.erase(i);
	}
}

void GUI::Container::draw(sf::RenderWindow& window) {
	std::for_each(objects.begin(), objects.end(), boost::bind(&Object::draw, _1, boost::ref(window)));
}

bool GUI::Container::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
	return std::find_if(objects.begin(), objects.end(), boost::bind(&Object::handleEvent, _1, boost::cref(event), boost::cref(window))) != objects.end();
}

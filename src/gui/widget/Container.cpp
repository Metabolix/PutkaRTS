/*
 * Widget container class implementation.
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

#include <algorithm>
#include <functional>

#include "Container.hpp"

GUI::Widget::Container::~Container() {
	clear();
}

void GUI::Widget::Container::insert(const std::shared_ptr<Widget>& object) {
	object->setContainer(this);
	objects.push_back(object);
}

void GUI::Widget::Container::erase(const std::shared_ptr<Widget>& object) {
	WidgetListType::iterator i = std::find(objects.begin(), objects.end(), object);
	if (i != objects.end()) {
		i->get()->setContainer(NULL);
		objects.erase(i);
	}
}

void GUI::Widget::Container::draw(sf::RenderWindow& window) {
	std::for_each(objects.rbegin(), objects.rend(), std::bind(&Widget::draw, std::placeholders::_1, std::ref(window)));
}

bool GUI::Widget::Container::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
	return std::find_if(objects.begin(), objects.end(), std::bind(&Widget::handleEvent, std::placeholders::_1, std::cref(event), std::cref(window))) != objects.end();
}

void GUI::Widget::Container::updateState(sf::RenderWindow& window) {
	std::for_each(objects.rbegin(), objects.rend(), std::bind(&Widget::updateState, std::placeholders::_1, std::ref(window)));
}

void GUI::Widget::Container::activate(const Widget * widget) {
	for (WidgetListType::iterator i = objects.begin(); i != objects.end(); ++i) {
		if (i->get() == widget) {
			std::shared_ptr<Widget> tmp = *i;
			objects.erase(i);
			objects.push_front(tmp);
			return;
		}
	}
}

void GUI::Widget::Container::clear() {
	while (!objects.empty()) {
		erase(*objects.begin());
	}
}

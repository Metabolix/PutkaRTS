/*
 * DropDown widget class implementation.
 *
 * Copyright 2011 Jaakko Puntila
 * Copyright 2011 Mika Katajamäki
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

#include "DropDown.hpp"
#include "Container.hpp"
#include "Label.hpp"

GUI::Widget::DropDown::DropDown(float x, float y, float width, float height, float dropDownMaxHeight_, CallbackType callback):
	Widget(x, y, width, height),
	dropButton("V", x + width - height + 2, y + 2, height - 4, height - 4, std::bind(&GUI::Widget::DropDown::dropButtonPressed, this)),
	dropList(x, y + height, width, dropDownMaxHeight_, std::bind(&GUI::Widget::DropDown::itemSelected, this, std::placeholders::_1)),
	open(false),
	action(callback) {
	dropList.setLineHeight(height - 2 * dropList.getBorderWidth());
	dropDownMaxHeight = dropDownMaxHeight_;
	calculateDropDownSize();
}

void GUI::Widget::DropDown::dropButtonPressed() {
	open = true;

	if (container) {
		container->activate(this);
	}
}

void GUI::Widget::DropDown::itemSelected(const List::Item& item) {
	open = false;

	if (action) {
		action(item);
	}
}

void GUI::Widget::DropDown::calculateDropDownSize() {
	float height = std::max(1, std::min<int>(dropList.countItems(), dropDownMaxHeight / dropList.getLineHeight()));
	height *= dropList.getLineHeight();
	height += 2 * dropList.getBorderWidth();

	dropList.setSize(position.width, height);
}

bool GUI::Widget::DropDown::handleEvent(const sf::Event& e, const sf::RenderWindow& window) {
	if (dropButton.handleEvent(e, window)) {
		return true;
	}

	if (open) {
		if (dropList.handleEvent(e, window)) {
			return true;
		} else if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
			open = false;
			return true;
		}
	}

	return false;
}

void GUI::Widget::DropDown::draw(sf::RenderWindow& window) {
	float bw = dropList.getBorderWidth();

	sf::RectangleShape tmp;
	tmp.setSize(sf::Vector2f(position.width - 2 * bw, position.height - 2 * bw));
	tmp.setPosition(position.left + bw, position.top + bw);
	tmp.setFillColor(Color::background);
	tmp.setOutlineColor(Color::border);
	tmp.setOutlineThickness(bw);
	window.draw(tmp);

	Label tmpText(
		dropList.getSelectedText(),
		position.left + 2 * bw,
		position.top + bw,
		position.width - 4 * bw,
		position.height - 2 * bw
	);
	tmpText.draw(window);
	dropButton.draw(window);

	if (open) {
		dropList.draw(window);
	}
}

const GUI::Widget::List::KeyType& GUI::Widget::DropDown::getSelectedKey() const {
	return dropList.getSelectedKey();
}

const std::string& GUI::Widget::DropDown::getSelectedText() const {
	return dropList.getSelectedText();
}

void GUI::Widget::DropDown::insertItem(const GUI::Widget::List::KeyType& key, const std::string& text) {
	dropList.insertItem(key, text);
	calculateDropDownSize();
}

void GUI::Widget::DropDown::eraseItem(const GUI::Widget::List::KeyType& key) {
	dropList.eraseItem(key);
}

void GUI::Widget::DropDown::selectItem(const GUI::Widget::List::KeyType& key) {
	dropList.selectItem(key);
}

/*
 * List widget class implementation.
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

#include "List.hpp"
#include "Label.hpp"

#include <algorithm>
#include <boost/bind.hpp>

GUI::Widget::List::Item GUI::Widget::List::invalidItem = {"", ""};

GUI::Widget::List::List(float x, float y, float width, float height, CallbackType callback):
	Widget(x, y, width, height),
	scrollbar(x + width - 16 - 4, y + 4, height - 2 * 4, 16, true),
	action(callback),
	lineHeight(20),
	borderWidth(4) {
	selected = items.end();
	scrollPosition = 0;
}

bool GUI::Widget::List::handleEvent(const sf::Event& e, const sf::RenderWindow& window) {
	const int lines = (position.GetHeight() - 2 * borderWidth) / lineHeight;

	if ((int)items.size() > lines) {
		if (scrollbar.handleEvent(e, window)) {
			setScrollPosition(((items.size() - lines) * scrollbar.getScrollPosition()) + 0.5f);
			return true;
		}
	}

	if (e.Type == sf::Event::MouseButtonPressed && e.MouseButton.Button == sf::Mouse::Left) {
		sf::Vector2f mouse(window.ConvertCoords(e.MouseButton.X, e.MouseButton.Y));

		sf::FloatRect itemArea = position;
		itemArea.Right -= borderWidth;
		itemArea.Left += borderWidth;
		itemArea.Top += borderWidth;
		itemArea.Bottom -= borderWidth;

		if (itemArea.Contains(mouse.x, mouse.y)) {
			int itemIndex = scrollPosition + (mouse.y - itemArea.Top) / lineHeight;

			selected = items.begin();
			for (int i = 0; i < itemIndex && selected != items.end(); i++) {
				selected++;
			}

			if (action) {
				action(*selected);
			}

			return true;
		}
	} else if (e.Type == sf::Event::MouseWheelMoved) {
		const sf::Input& input = window.GetInput();
		sf::Vector2f mouse(window.ConvertCoords(input.GetMouseX(), input.GetMouseY()));

		sf::FloatRect itemArea = position;
		itemArea.Right -= borderWidth;
		itemArea.Left += borderWidth;
		itemArea.Top += borderWidth;
		itemArea.Bottom -= borderWidth;

		if (itemArea.Contains(mouse.x, mouse.y)) {
			scroll(-e.MouseWheel.Delta);
			scrollbar.setScrollPosition((float)scrollPosition / (items.size() - lines));
			return true;
		}
	}

	return false;
}

void GUI::Widget::List::draw(sf::RenderWindow& window) {
	int lines = (position.GetHeight() - 2 * borderWidth) / lineHeight;

	window.Draw(
		sf::Shape::Rectangle(
			position.Left + borderWidth,
			position.Top + borderWidth,
			position.Right - borderWidth,
			position.Bottom - borderWidth,
			Color::background,
			borderWidth,
			Color::border
		)
	);

	int counter = 0;

	const sf::Input& input = window.GetInput();
	sf::Vector2f mouse(window.ConvertCoords(input.GetMouseX(), input.GetMouseY()));

	int hoverIndex = -1;

	sf::FloatRect itemArea = position;
	itemArea.Right -= borderWidth + 16;
	itemArea.Left += borderWidth;
	itemArea.Top += borderWidth;
	itemArea.Bottom -= borderWidth;

	if (itemArea.Contains(mouse.x, mouse.y)) {
		hoverIndex = scrollPosition + (mouse.y - itemArea.Top) / lineHeight;
	}

	for (ItemContainerType::const_iterator i = items.begin(); i != items.end() && counter < lines + scrollPosition; ++i, ++counter) {
		if (counter < scrollPosition) {
			continue;
		}

		if (selected == i) {
			window.Draw(
				sf::Shape::Rectangle(
					position.Left + borderWidth,
					position.Top + borderWidth + (counter - scrollPosition) * lineHeight,
					position.Right - borderWidth,
					position.Top + borderWidth + (counter + 1 - scrollPosition) * lineHeight,
					Color::backgroundHover
				)
			);
		} else if (counter == hoverIndex) {
			window.Draw(
				sf::Shape::Rectangle(
					position.Left + borderWidth,
					position.Top + borderWidth + (counter - scrollPosition) * lineHeight,
					position.Right - borderWidth,
					position.Top + borderWidth + (counter + 1 - scrollPosition) * lineHeight,
					Color::backgroundHover
				)
			);
		}

		Label tmpText(
			i->text,
			position.Left + 2 * borderWidth,
			position.Top + borderWidth + (counter - scrollPosition) * lineHeight,
			lineHeight
		);
		tmpText.draw(window);
	}

	if ((int)items.size() > lines) {
		scrollbar.draw(window);
	}
}

void GUI::Widget::List::setScrollPosition(int pos) {
	int lines = (position.GetHeight() - 2 * borderWidth) / lineHeight;

	pos = std::max(0, std::min(pos, (int)(items.size()-lines)));
	scrollPosition = pos;
}

void GUI::Widget::List::scroll(int n) {
	setScrollPosition(scrollPosition+n);
}

const GUI::Widget::List::KeyType& GUI::Widget::List::getSelectedKey() const {
	if (selected != items.end()) {
		return selected->key;
	}

	return invalidItem.key;
}

const std::string& GUI::Widget::List::getSelectedText() const {
	if (selected != items.end()) {
		return selected->text;
	}

	return invalidItem.text;
}

void GUI::Widget::List::insertItem(const GUI::Widget::List::KeyType& key, const std::string& text) {
	Item tmp;
	tmp.key = key;
	tmp.text = text;

	for (ItemContainerType::iterator i = items.begin(); i != items.end(); ++i) {
		if ( i->key == key ) {
			i->text = text;
			return;
		}
	}

	items.push_back(tmp);
}

void GUI::Widget::List::eraseItem(const GUI::Widget::List::KeyType& key) {
	for (ItemContainerType::iterator i = items.begin(); i != items.end(); ++i) {
		if (i->key == key) {
			if (selected == i) {
				selected == items.end();
			}

			items.erase(i);
			return;
		}
	}
}

float GUI::Widget::List::getBorderWidth() const {
	return borderWidth;
}

float GUI::Widget::List::getLineHeight() const {
	return lineHeight;
}

void GUI::Widget::List::setLineHeight(float lineHeight_) {
	lineHeight = std::max(1.0f, std::min(lineHeight_, position.GetHeight() - 2 * borderWidth));
}

unsigned GUI::Widget::List::countItems() const {
	return items.size();
}

void GUI::Widget::List::move(float dx, float dy) {
	Widget::move(dx, dy);
	scrollbar.move(dx, dy);
}

void GUI::Widget::List::setSize(float width, float height) {
	scrollbar.move(width - position.GetWidth(), 0);
	scrollbar.setSize(16, height - 2 * getBorderWidth());
	Widget::setSize(width, height);
}

void GUI::Widget::List::selectItem(const GUI::Widget::List::KeyType& key) {
	for (ItemContainerType::const_iterator i = items.begin(); i != items.end(); ++i) {
		if (i->key == key) {
			selected = i;
			return;
		}
	}
}

#include <algorithm>
#include <functional>

#include "List.hpp"
#include "Label.hpp"

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
	const int lines = (position.height - 2 * borderWidth) / lineHeight;

	if ((int)items.size() > lines) {
		if (scrollbar.handleEvent(e, window)) {
			setScrollPosition(((items.size() - lines) * scrollbar.getScrollPosition()) + 0.5f);
			return true;
		}
	}

	if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
		sf::Vector2f mouse(window.mapPixelToCoords(sf::Vector2i(e.mouseButton.x, e.mouseButton.y)));

		sf::FloatRect itemArea = position;
		itemArea.left += borderWidth;
		itemArea.top += borderWidth;
		itemArea.width -= 2 * borderWidth;
		itemArea.height -= 2 * borderWidth;

		if (itemArea.contains(mouse)) {
			int itemIndex = scrollPosition + (mouse.y - itemArea.top) / lineHeight;

			selected = items.begin();
			for (int i = 0; i < itemIndex && selected != items.end(); i++) {
				selected++;
			}

			if (action && selected != items.end()) {
				action(*selected);
			}

			return true;
		}
	} else if (e.type == sf::Event::MouseWheelMoved) {
		sf::Vector2f mouse(window.mapPixelToCoords(sf::Mouse::getPosition(window)));

		sf::FloatRect itemArea = position;
		itemArea.left += borderWidth;
		itemArea.top += borderWidth;
		itemArea.width -= 2 * borderWidth;
		itemArea.height -= 2 * borderWidth;

		if (itemArea.contains(mouse)) {
			scroll(-e.mouseWheel.delta);
			scrollbar.setScrollPosition((float)scrollPosition / (items.size() - lines));
			return true;
		}
	}

	return false;
}

void GUI::Widget::List::draw(sf::RenderWindow& window) {
	int lines = (position.height - 2 * borderWidth) / lineHeight;

	const int bw = borderWidth;
	sf::RectangleShape tmp;
	tmp.setSize(sf::Vector2f(position.width - 2 * bw, position.height - 2 * bw));
	tmp.setPosition(position.left + bw, position.top + bw);
	tmp.setFillColor(Color::background);
	tmp.setOutlineColor(Color::border);
	tmp.setOutlineThickness(bw);
	window.draw(tmp);

	sf::FloatRect itemArea = position;
	itemArea.left += borderWidth;
	itemArea.top += borderWidth;
	itemArea.width -= 2 * borderWidth + 16;
	itemArea.height -= 2 * borderWidth;

	sf::Vector2f mouse(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
	int hoverIndex = -1;
	if (itemArea.contains(mouse)) {
		hoverIndex = scrollPosition + (mouse.y - itemArea.top) / lineHeight;
	}

	int counter = 0;
	for (ItemContainerType::const_iterator i = items.begin(); i != items.end() && counter < lines + scrollPosition; ++i, ++counter) {
		if (counter < scrollPosition) {
			continue;
		}

		if (selected == i || counter == hoverIndex) {
			sf::RectangleShape tmp;
			tmp.setSize(sf::Vector2f(position.width - 2 * borderWidth, lineHeight));
			tmp.setPosition(position.left + borderWidth, position.top + borderWidth + (counter - scrollPosition) * lineHeight);
			tmp.setFillColor(Color::backgroundHover);
			window.draw(tmp);
		}

		Label tmpText(
			i->text,
			position.left + 2 * borderWidth,
			position.top + borderWidth + (counter - scrollPosition) * lineHeight,
			lineHeight
		);
		tmpText.draw(window);
	}

	if ((int)items.size() > lines) {
		scrollbar.draw(window);
	}
}

void GUI::Widget::List::setScrollPosition(int pos) {
	int lines = (position.height - 2 * borderWidth) / lineHeight;

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
				selected = items.end();
			}

			items.erase(i);
			return;
		}
	}
}

void GUI::Widget::List::clear() {
	items.clear();
	selected = items.end();
}

float GUI::Widget::List::getBorderWidth() const {
	return borderWidth;
}

float GUI::Widget::List::getLineHeight() const {
	return lineHeight;
}

void GUI::Widget::List::setLineHeight(float lineHeight_) {
	lineHeight = std::max(1.0f, std::min(lineHeight_, position.height - 2 * borderWidth));
}

unsigned GUI::Widget::List::countItems() const {
	return items.size();
}

void GUI::Widget::List::move(float dx, float dy) {
	Widget::move(dx, dy);
	scrollbar.move(dx, dy);
}

void GUI::Widget::List::setSize(float width, float height) {
	scrollbar.move(width - position.width, 0);
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

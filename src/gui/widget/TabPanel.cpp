#include <functional>

#include "TabPanel.hpp"

GUI::Widget::TabPanel::TabPanel(float x, float y, float width, float height):
	TabManager(x, y, width, height) {
}

bool GUI::Widget::TabPanel::handleEvent(const sf::Event& e, const sf::RenderWindow& window) {
	for (ButtonListType::iterator i = buttons.begin(); i != buttons.end(); ++i) {
		if (i->button->handleEvent(e, window)) {
			return true;
		}
	}

	return TabManager::handleEvent(e, window);
}

void GUI::Widget::TabPanel::draw(sf::RenderWindow& window) {
	TabManager::draw(window);

	for (ButtonListType::const_iterator i = buttons.begin(); i != buttons.end(); ++i) {
		i->button->draw(window, (i->key == selectedTabKey));
	}
}

void GUI::Widget::TabPanel::addButton(const std::string& label, const GUI::Widget::TabManager::TabKeyType& key) {
	Button tmp;
	tmp.button.reset(new GUI::Widget::Button(label, 0.0f, 0.0f, 1.0f, 1.0f, std::bind(&GUI::Widget::TabManager::selectTab, this, key)));
	tmp.key = key;

	buttons.push_back(tmp);
	resizeButtons();
}

void GUI::Widget::TabPanel::removeButton(const GUI::Widget::TabManager::TabKeyType& key) {
	for (ButtonListType::iterator i = buttons.begin(); i != buttons.end(); ++i) {
		if (i->key == key) {
			i = buttons.erase(i);
		}
	}
	resizeButtons();
}

void GUI::Widget::TabPanel::resizeButtons() {
	//TODO: Panel scrolling if need for it arises.

	if (buttons.empty()) {
		return;
	}

	float buttonWidth = position.width / buttons.size();
	int count = 0;

	for (ButtonListType::iterator i = buttons.begin(); i != buttons.end(); ++i) {
		i->button->setPosition(position.left + count * buttonWidth, position.top, buttonWidth, position.height);
		count++;
	}
}

void GUI::Widget::TabPanel::updateState(sf::RenderWindow& window) {
	for (ButtonListType::iterator i = buttons.begin(); i != buttons.end(); ++i) {
		i->button->updateState(window);
	}

	TabManager::updateState(window);
}

GUI::Widget::TabPanel::TabContainerPtrType GUI::Widget::TabPanel::addTab(const TabKeyType& key, const std::string& title, TabContainerPtrType container) {
	TabContainerPtrType ret = TabManager::addTab(key, container);
	addButton(title, key);

	return ret;
}

void GUI::Widget::TabPanel::removeTab(const TabKeyType& key) {
	TabManager::removeTab(key);
	removeButton(key);
}

#include <stdexcept>

#include "TabManager.hpp"

GUI::Widget::TabManager::TabManager() {
}

GUI::Widget::TabManager::TabManager(float x, float y, float width, float height):
	Widget(x, y, width, height) {
}

bool GUI::Widget::TabManager::handleEvent(const sf::Event& e, const sf::RenderWindow& window) {
	TabListType::iterator selectedTab = tabs.find(selectedTabKey);

	if (selectedTab != tabs.end() && selectedTab->second) {
		return selectedTab->second->handleEvent(e, window);
	}

	return false;
}

void GUI::Widget::TabManager::draw(sf::RenderWindow& window) {
	TabListType::iterator selectedTab = tabs.find(selectedTabKey);

	if (selectedTab != tabs.end() && selectedTab->second) {
		selectedTab->second->draw(window);
	}
}

void GUI::Widget::TabManager::updateState(sf::RenderWindow& window) {
	for (TabListType::iterator i = tabs.begin(); i != tabs.end(); ++i) {
		if (i->second) {
			i->second->updateState(window);
		}
	}
}

GUI::Widget::TabManager::TabContainerPtrType GUI::Widget::TabManager::addTab(const TabKeyType& key, TabContainerPtrType container) {
	removeTab(key);

	if (!container) {
		//Create container if one was not given.
		container.reset(new GUI::Widget::Container());
	}

	tabs[key] = container;

	if (tabs.size() == 1) {
		selectedTabKey = tabs.begin()->first;
	}

	return container;
}

GUI::Widget::TabManager::TabContainerPtrType GUI::Widget::TabManager::getTab(const TabKeyType& key) {
	TabListType::iterator tab = tabs.find(key);

	if (tab == tabs.end()) {
		throw std::runtime_error("GUI::Widget::TabManager::getTab(): Invalid key.");
	}

	return tab->second;
}

void GUI::Widget::TabManager::removeTab(const TabKeyType& key) {
	tabs.erase(key);

	if (!tabs.empty() && key == selectedTabKey) {
		selectedTabKey = tabs.begin()->first;
	}
}

void GUI::Widget::TabManager::selectTab(const TabKeyType& key) {
	TabListType::iterator tab = tabs.find(key);

	if (tab != tabs.end()) {
		selectedTabKey = key;
	}
}

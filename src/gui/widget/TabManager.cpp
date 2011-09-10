/*
 * Tab manager widget class implementation.
 *
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

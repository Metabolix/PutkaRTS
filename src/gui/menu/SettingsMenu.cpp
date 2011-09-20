/*
 * Settings menu class implementation.
 *
 * Copyright 2011 Lauri Kenttä
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

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

#include "util/Path.hpp"

#include "SettingsMenu.hpp"
#include "gui/GUI.hpp"

#include "gui/widget/Button.hpp"
#include "gui/widget/Checkbox.hpp"
#include "gui/widget/DropDown.hpp"
#include "gui/widget/Slider.hpp"
#include "gui/widget/Label.hpp"

GUI::Menu::SettingsMenu::SettingsMenu(boost::shared_ptr<Widget> parent_, sf::RenderWindow& window) :
	Menu(parent_),
	tabManager(new GUI::Widget::TabPanel(50, 100, 200, 16)) {

	insert(tabManager);
	insert(new GUI::Widget::Button("Accept", 400, 300, 100, 30, boost::bind(&GUI::Menu::SettingsMenu::applyChanges, this, boost::ref(window))));
	insert(new GUI::Widget::Button("Cancel", 400, 340, 100, 30, boost::bind(&GUI::Menu::SettingsMenu::closeMenu, this)));

	buildGraphicsTab(1);
	buildInputTab(2);
}

void GUI::Menu::SettingsMenu::buildGraphicsTab(const GUI::Widget::TabPanel::TabKeyType& key) {

	//Get current settings.
	fullscreen = GUI::config.getBool("window.fullscreen", false);

	//List video modes.
	boost::shared_ptr<GUI::Widget::DropDown> videoModeList(new GUI::Widget::DropDown(100, 200, 200, 25, 200, boost::bind(&GUI::Menu::SettingsMenu::setVideoMode, this, _1)));
	for (unsigned i = 0; i < sf::VideoMode::GetModesCount(); i++) {
		const sf::VideoMode& mode = sf::VideoMode::GetMode(i);

		if (mode.BitsPerPixel != 32) {
			continue;
		}

		if (mode.Width > desktopMode.Width || mode.Height > desktopMode.Height) {
			continue;
		}

		boost::format text("%1%x%2%");
		text % mode.Width % mode.Height;

		boost::format key("%1%");
		key % i;

		videoModeList->insertItem(key.str(), text.str());

		if (mode.Width == window.GetWidth() && mode.Height == window.GetHeight()) {
			videoModeList->selectItem(key.str());
			videoMode = atoi(key.str().c_str());
		}
	}

	//create tab
	tabManager->addTab(key, "Graphics");

	//insert resolution list
	tabManager->getTab(key)->insert(videoModeList);
	//insert fullscreen box
	tabManager->getTab(key)->insert(new GUI::Widget::Checkbox(100, 150, 20, 20, fullscreen, boost::bind(&GUI::Menu::SettingsMenu::setFullScreen, this, _1)));
	tabManager->getTab(key)->insert(new GUI::Widget::Label("Fullscreen (needs restart to take effect)", 125, 150, 20));
}

void GUI::Menu::SettingsMenu::buildInputTab(const GUI::Widget::TabPanel::TabKeyType& key) {

	//get current settings.
	zoomSpeed = GUI::config.getDouble("gameUI.zoomSpeed", 2);
	keyboardScrollSpeed = GUI::config.getDouble("gameUI.keyboardScrollSpeed", 1);
	borderScrollSpeed = GUI::config.getDouble("gameUI.borderScrollSpeed", 1);
	reverseDrag = GUI::config.getBool("gameUI.reverseDrag", false);

	//create tab
	tabManager->addTab(key, "Input");

	//zoomSpeed
	tabManager->getTab(key)->insert(new GUI::Widget::Label("Zooming speed", 100, 150, 16));
	tabManager->getTab(key)->insert(new GUI::Widget::Slider(100, 170, 200, 15, false, boost::bind(&GUI::Menu::SettingsMenu::setZoomSpeed, this, _1), 0.5f, 4.0f, zoomSpeed));
	//keyboardScrollSpeed
	tabManager->getTab(key)->insert(new GUI::Widget::Label("Keyboard scrolling speed", 100, 200, 16));
	tabManager->getTab(key)->insert(new GUI::Widget::Slider(100, 220, 200, 15, false, boost::bind(&GUI::Menu::SettingsMenu::setKeyboardScrollSpeed, this, _1), 0.5f, 2.0f, keyboardScrollSpeed));
	//borderScrollSpeed
	tabManager->getTab(key)->insert(new GUI::Widget::Label("Border scrolling speed", 100, 250, 16));
	tabManager->getTab(key)->insert(new GUI::Widget::Slider(100, 270, 200, 15, false, boost::bind(&GUI::Menu::SettingsMenu::setBorderScrollSpeed, this, _1), 0.5f, 2.0f, borderScrollSpeed));
	//reverseDrag
	tabManager->getTab(key)->insert(new GUI::Widget::Checkbox(100, 300, 20, 20, fullscreen, boost::bind(&GUI::Menu::SettingsMenu::setReverseDrag, this, _1)));
	tabManager->getTab(key)->insert(new GUI::Widget::Label("Reverse dragging", 125, 300, 20));
}

void GUI::Menu::SettingsMenu::applyChanges(sf::RenderWindow& window) {
	sf::VideoMode mode = sf::VideoMode::GetMode(videoMode);

	if (!mode.IsValid()) {
		fullscreen = false;
	}

	if (mode.Width != window.GetWidth() || mode.Height != window.GetHeight()) {
		GUI::config.setInt("window.size.x", mode.Width);
		GUI::config.setInt("window.size.y", mode.Height);
	}

	if (fullscreen || GUI::config.getBool("window.fullscreen", false)) {
		GUI::config.setBool("window.fullscreen", fullscreen);
		GUI::createWindow();
	}

	if (mode.Width != window.GetWidth() || mode.Height != window.GetHeight()) {
		window.SetSize(mode.Width, mode.Height);
		window.GetDefaultView().SetFromRect(sf::FloatRect(0, 0, mode.Width, mode.Height));
	}

	GUI::config.setDouble("gameUI.zoomSpeed", zoomSpeed);
	GUI::config.setDouble("gameUI.keyboardScrollSpeed", keyboardScrollSpeed);
	GUI::config.setDouble("gameUI.borderScrollSpeed", borderScrollSpeed);
	GUI::config.setBool("gameUI.reverseDrag", reverseDrag);

	GUI::config.save();
	closeMenu();
}

void GUI::Menu::SettingsMenu::setFullScreen(bool state) {
	fullscreen = state;
}

void GUI::Menu::SettingsMenu::setVideoMode(const GUI::Widget::List::Item& item) {
	if (item.key.empty()) {
		return;
	}

	// Non-empty keys are valid.
	videoMode = boost::lexical_cast<std::size_t>(item.key.c_str());
}

void GUI::Menu::SettingsMenu::setBorderScrollSpeed(float value) {
	borderScrollSpeed = value;
}

void GUI::Menu::SettingsMenu::setKeyboardScrollSpeed(float value) {
	keyboardScrollSpeed = value;
}

void GUI::Menu::SettingsMenu::setZoomSpeed(float value) {
	zoomSpeed = value;
}

void GUI::Menu::SettingsMenu::setReverseDrag(bool state) {
	reverseDrag = state;
}

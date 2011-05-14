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

#include "SettingsMenu.hpp"
#include "gui/GUI.hpp"

#include "gui/widget/Button.hpp"
#include "gui/widget/Checkbox.hpp"
#include "gui/widget/DropDown.hpp"

#include "util/Path.hpp"

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

GUI::Menu::SettingsMenu::SettingsMenu(sf::RenderWindow& window, boost::shared_ptr<Widget> parent_) :
	Menu(parent_),
	tabManager(new GUI::Widget::TabPanel(50, 100, 200, 16)) {

	insert(tabManager);
	insert(new GUI::Widget::Button("Accept", 400, 300, 100, 30, boost::bind(&GUI::Menu::SettingsMenu::applyChanges, this, boost::ref(window))));
	insert(new GUI::Widget::Button("Cancel", 400, 340, 100, 30, boost::bind(&GUI::Menu::SettingsMenu::closeMenu, this)));

	buildGraphicsTab(1);
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
	tabManager->getTab(key)->insert(new GUI::Widget::Checkbox("Fullscreen (needs restart to take effect)", 100, 150, 20, 20, fullscreen, boost::bind(&GUI::Menu::SettingsMenu::setFullScreen, this, _1)));
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

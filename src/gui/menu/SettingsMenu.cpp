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

GUI::Menu::SettingsMenu::SettingsMenu(sf::RenderWindow& window, boost::shared_ptr<Widget> parent_) :
	Menu(parent_) {

	//Get current settings.
	fullscreen = GUI::config.getBool("window.fullscreen", false);
	int res_x = GUI::config.getInt("window.size.x", 800);
	int res_y = GUI::config.getInt("window.size.y", 600);

	//List video modes.
	boost::shared_ptr<GUI::Widget::DropDown> videoModeList(new GUI::Widget::DropDown(100, 150, 200, 25, 200, boost::bind(&GUI::Menu::SettingsMenu::setVideoMode, this, _1)));
	for (unsigned i = 0; i < sf::VideoMode::GetModesCount(); i++) {
		const sf::VideoMode& mode = sf::VideoMode::GetMode(i);

		if (mode.BitsPerPixel != 32) {
			continue;
		}

		if (mode.Width > sf::VideoMode::GetDesktopMode().Width || mode.Height > sf::VideoMode::GetDesktopMode().Height) {
			continue;
		}

		boost::format text("%1%x%2%");
		text % mode.Width % mode.Height;

		boost::format key("%1%");
		key % i;

		videoModeList->insertItem(key.str(), text.str());

		if (mode.Width == res_x && mode.Height == res_y) {
			videoModeList->selectItem(key.str());
			videoMode = atoi(key.str().c_str());
		}
	}

	//Build GUI
	insert(videoModeList);
	insert(new GUI::Widget::Checkbox("Fullscreen (needs restart to take effect)", 100, 100, 20, 20, fullscreen, boost::bind(&GUI::Menu::SettingsMenu::setFullScreen, this, _1)));
	insert(new GUI::Widget::Button("Accept", 400, 300, 100, 30, boost::bind(&GUI::Menu::SettingsMenu::applyChanges, this, boost::ref(window))));
	insert(new GUI::Widget::Button("Cancel", 400, 340, 100, 30, boost::bind(&GUI::Menu::SettingsMenu::closeMenu, this)));
}

void GUI::Menu::SettingsMenu::applyChanges(sf::RenderWindow& window) {
	sf::VideoMode mode = sf::VideoMode::GetMode(videoMode);

	bool resize;

	if (mode.Width != GUI::config.getInt("window.size.x", 800) || mode.Height != GUI::config.getInt("window.size.y", 600)) {
		GUI::config.setInt("window.size.x", mode.Width);
		GUI::config.setInt("window.size.y", mode.Height);
		resize = true;
	}

	if (fullscreen != GUI::config.getBool("window.fullscreen", false)) {
		GUI::config.setBool("window.fullscreen", fullscreen);
		resize = false;
	}

	if (resize) {
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

	int key = atoi(item.key.c_str());

	if (key >= 0 && key < sf::VideoMode::GetModesCount()) {
		videoMode = key;
	}
}

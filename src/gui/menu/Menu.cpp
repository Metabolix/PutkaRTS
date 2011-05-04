/*
 * Menu class implementation.
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

#include "Menu.hpp"

#include "gui/GUI.hpp"
#include "util/Path.hpp"

#include <boost/bind.hpp>

GUI::ImageCache GUI::Menu::Menu::images;

GUI::Menu::Menu::Menu(boost::shared_ptr<Widget> _parent) {
	const sf::Image& logoImage(images.get(Path::findDataPath("graphics/logo.png")));
	logoSprite.SetImage(logoImage);
	logoSprite.SetCenter(logoImage.GetWidth() / 2, 0);
	logoSprite.SetPosition(320, 1);

	openMenu(_parent);
}

void GUI::Menu::Menu::openMenu(boost::shared_ptr<Widget> _parent) {
	menuIsOpen = true;
	parent = _parent;
}

void GUI::Menu::Menu::closeMenu() {
	menuIsOpen = false;
	if (currentWidget.get() == this) {
		GUI::currentWidget = parent;
	}
}

void GUI::Menu::Menu::draw(sf::RenderWindow& window) {
	// Make view that is as close to 640x480 as possible and centered.
	view = window.GetDefaultView();
	view.Zoom(std::min(view.GetRect().GetWidth() / 640, view.GetRect().GetHeight() / 480));
	view.SetCenter(320, 240);
	window.SetView(view);

	window.Clear(sf::Color(0xcc, 0x66, 0x33));
	Container::draw(window);
	window.Draw(logoSprite);
	window.Display();
}

bool GUI::Menu::Menu::isOpen() const {
	return menuIsOpen;
}

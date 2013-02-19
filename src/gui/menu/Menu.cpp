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

#include <boost/bind.hpp>

#include "util/Path.hpp"

#include "Menu.hpp"
#include "gui/GUI.hpp"

GUI::TextureCache GUI::Menu::Menu::textures;

GUI::Menu::Menu::Menu(boost::shared_ptr<Widget> parent_) {
	const sf::Texture& logoTexture(textures.get(Path::findDataPath("graphics/logo.png")));
	logoSprite.setTexture(logoTexture);
	logoSprite.setOrigin(logoTexture.getSize().x / 2, 0);
	logoSprite.setPosition(320, 1);

	openMenu(parent_);
}

void GUI::Menu::Menu::openMenu(boost::shared_ptr<Widget> parent_) {
	menuIsOpen = true;
	parent = parent_;
}

void GUI::Menu::Menu::closeMenu() {
	menuIsOpen = false;
	if (currentWidget.get() == this) {
		GUI::currentWidget = parent;
	}
}

void GUI::Menu::Menu::draw(sf::RenderWindow& window) {
	// Make view that is as close to 640x480 as possible and centered.
	view = window.getDefaultView();
	view.zoom(1.0f / std::min(view.getSize().x / 640, view.getSize().y / 480));
	view.setCenter(320, 240);
	window.setView(view);

	window.clear(sf::Color(0xcc, 0x66, 0x33));
	Container::draw(window);
	window.draw(logoSprite);
}

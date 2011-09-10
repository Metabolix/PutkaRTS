/*
 * Global variables for the GUI version of the game.
 *
 * Copyright 2011 Lauri Kenttä
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

#include "ProgramInfo.hpp"

#include "GUI.hpp"

Configuration GUI::config;

boost::shared_ptr<GUI::Widget::Widget> GUI::currentWidget;

sf::RenderWindow GUI::window;

const sf::VideoMode GUI::desktopMode = sf::VideoMode::GetDesktopMode();

void GUI::createWindow() {
	sf::VideoMode mode = sf::VideoMode(
		GUI::config.getInt("window.size.x", 800),
		GUI::config.getInt("window.size.y", 600)
	);
	unsigned long style = sf::Style::Close;

	if (GUI::config.getBool("window.fullscreen", false)) {
		style = sf::Style::Fullscreen;
		if (!mode.IsValid()) {
			mode = desktopMode;
		}
	} else if (mode.Width > desktopMode.Width || mode.Height > desktopMode.Height) {
		//Scale down the window size if larger than desktop resolution.
		float scale = std::max((float)mode.Width / desktopMode.Width, (float)mode.Height / desktopMode.Height);
		mode.Width /= scale;
		mode.Height /= scale;
	}

	std::string title = ProgramInfo::name + " (version " + ProgramInfo::version + ", GUI)";
	window.Create(mode, title, style);
	window.SetFramerateLimit(GUI::config.getInt("window.framerate", 60));
}

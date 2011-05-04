/*
 * The GUI program entry point.
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

#include <iostream>
#include <stdexcept>

#include <SFML/Graphics.hpp>

#include "ProgramInfo.hpp"
#include "GUI.hpp"

#include "util/Path.hpp"

#include "menu/MainMenu.hpp"
#include "game/GameHandler.hpp"

namespace GUI {
	int main(int argc, char **argv);
}

/**
 * The main function needed for compiling.
 */
int main(int argc, char **argv) {
	return GUI::main(argc, argv);
}

/**
 * Main function for the graphical program.
 */
int GUI::main(int argc, char **argv)
try {
	Path::init(argc ? argv[0] : "./bin/unknown.exe");

	std::string configPath = Path::getConfigPath("gui.conf");
	GUI::config.load(configPath);

	sf::VideoMode mode = sf::VideoMode(
		GUI::config.getInt("window.size.x", 800),
		GUI::config.getInt("window.size.y", 600)
	);
	unsigned long style = sf::Style::Close;

	if (GUI::config.getBool("window.fullscreen", false)) {
		style = sf::Style::Fullscreen;
		if (!mode.IsValid()) {
			mode = sf::VideoMode::GetDesktopMode();
		}
	} else if (mode.Width > sf::VideoMode::GetDesktopMode().Width || mode.Height > sf::VideoMode::GetDesktopMode().Height) {
		//Scale down the window size if larger than desktop resolution.
		float scale = std::max((float)mode.Width / sf::VideoMode::GetDesktopMode().Width, (float)mode.Height / sf::VideoMode::GetDesktopMode().Height);
		mode.Width /= scale;
		mode.Height /= scale;
	}

	std::string title = ProgramInfo::name + " (version " + ProgramInfo::version + ", GUI)";
	window.Create(mode, title, style);

	window.SetFramerateLimit(GUI::config.getInt("window.framerate", 60));

	while (window.IsOpened()) {
		// If nothing is running, start the menu.
		if (!currentWidget) {
			currentWidget.reset(new Menu::MainMenu(window));
		}

		// Make another pointer to avoid deleting the widget prematurely.
		boost::shared_ptr<Widget::Widget> widget(currentWidget);

		sf::Event e;
		if (window.GetEvent(e)) {
			if (widget->handleEvent(e, window)) {
				continue;
			}
			if (e.Type == sf::Event::Closed || (e.Type == sf::Event::KeyPressed && e.Key.Code == sf::Key::Escape)) {
				window.Close();
				continue;
			}
		} else {
			widget->draw(window);
		}
	}
	currentWidget.reset();

	try {
		GUI::config.save(configPath);
	} catch (std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
} catch (std::exception& e) {
	std::cerr << "Fatal exception: " << e.what() << std::endl;
	return 1;
} catch (...) {
	std::cerr << "Fatal exception of unknown cause!" << std::endl;
	return 1;
}

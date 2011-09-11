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

#include "util/Path.hpp"

#include "GUI.hpp"

#include "menu/MainMenu.hpp"

#include <SFML/Graphics.hpp>

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

	GUI::createWindow();

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
			widget->updateState(window);
			widget->draw(window);
			window.Display();
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

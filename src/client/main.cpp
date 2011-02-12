/*
 * The client program entry point.
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
#include "util/Configuration.hpp"

#include "menu/MainMenuHandler.hpp"
#include "game/GameHandler.hpp"

/**
 * Main function for the game client.
 */
int main()
try {
	// TODO: try different local paths for different operating systems.
	Configuration conf("local/client.conf");

	sf::VideoMode mode = sf::VideoMode(
		conf.getInt("window.size.x", 800),
		conf.getInt("window.size.y", 600)
	);
	unsigned long style = sf::Style::Close;

	if (conf.getBool("window.fullscreen", false)) {
		style = sf::Style::Fullscreen;
		if (!mode.IsValid()) {
			mode = sf::VideoMode::GetDesktopMode();
		}
	}

	std::string title = ProgramInfo::name + " " + ProgramInfo::version + " (client mode)";
	sf::RenderWindow window(mode, title, style);

	MainMenuHandler menu;

	while (window.IsOpened()) {
		menu.run(window);
		if (!window.IsOpened()) {
			break;
		}
		if (GameHandler::instance.get()) {
			GameHandler::instance->run(window);
		}
	}
	try {
		conf.save();
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

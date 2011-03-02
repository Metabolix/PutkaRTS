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
#include "Client.hpp"

#include "util/Path.hpp"

#include "menu/MainMenuHandler.hpp"
#include "game/GameHandler.hpp"

/**
 * Main function for the game client.
 */
int main(int argc, char **argv)
try {
	Path::init(argc ? argv[0] : "./bin/unknown.exe");

	std::string configPath = Path::getConfigPath("client.conf");
	Client::config.load(configPath);

	sf::VideoMode mode = sf::VideoMode(
		Client::config.getInt("window.size.x", 800),
		Client::config.getInt("window.size.y", 600)
	);
	unsigned long style = sf::Style::Close;

	if (Client::config.getBool("window.fullscreen", false)) {
		style = sf::Style::Fullscreen;
		if (!mode.IsValid()) {
			mode = sf::VideoMode::GetDesktopMode();
		}
	}

	std::string title = ProgramInfo::name + " client (version: " + ProgramInfo::version + ")";
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
		Client::config.save(configPath);
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

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

#include "menu/MainMenuHandler.hpp"
#include "game/GameHandler.hpp"

/**
 * Main function for the game.
 */
int main(int argc, char **argv)
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
	}

	std::string title = ProgramInfo::name + " (version " + ProgramInfo::version + ", GUI)";
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
#include "ProgramInfo.hpp"

#include "GUI.hpp"

Configuration GUI::config;

std::shared_ptr<GUI::Widget::Widget> GUI::currentWidget;

sf::RenderWindow GUI::window;

const sf::VideoMode GUI::desktopMode = sf::VideoMode::getDesktopMode();

float GUI::frameTime;

void GUI::createWindow() {
	sf::VideoMode mode = sf::VideoMode(
		GUI::config.getInt("window.size.x", 800),
		GUI::config.getInt("window.size.y", 600)
	);
	unsigned long style = sf::Style::Close;

	if (GUI::config.getBool("window.fullscreen", false)) {
		style = sf::Style::Fullscreen;
		if (!mode.isValid()) {
			mode = desktopMode;
		}
	} else if (mode.width > desktopMode.width || mode.height > desktopMode.height) {
		//Scale down the window size if larger than desktop resolution.
		float scale = std::max((float)mode.width / desktopMode.width, (float)mode.height / desktopMode.height);
		mode.width /= scale;
		mode.height /= scale;
	}

	std::string title = ProgramInfo::name + " (version " + ProgramInfo::version + ", GUI)";
	window.create(mode, title, style);
	window.setFramerateLimit(GUI::config.getInt("window.framerate", 60));
}

#ifndef PUTKARTS_GUI_HPP
#define PUTKARTS_GUI_HPP

#include <memory>

#include "util/Configuration.hpp"
#include "gui/widget/Widget.hpp"

#include <SFML/Graphics.hpp>

/**
 * Namespace for everything that is specific to the graphical program.
 */
namespace GUI {
	/** Configuration. */
	extern Configuration config;

	/** The current Widget. */
	extern std::shared_ptr<Widget::Widget> currentWidget;

	/** The window for the game */
	extern sf::RenderWindow window;

	/** The current desktop video mode. Hope it doesn't change. */
	extern const sf::VideoMode desktopMode;

	/** The time spent in the previous frame. */
	extern float frameTime;

	/**
	 * Create (or recreate) the main window.
	 */
	void createWindow();
}

#endif

#ifndef PUTKARTS_GUI_Menu_MainMenu_HPP
#define PUTKARTS_GUI_Menu_MainMenu_HPP

#include "gui/menu/Menu.hpp"

#include <SFML/Graphics.hpp>

namespace GUI {
	namespace Menu {
		class MainMenu;
	}
}

/**
 * Main class for the game menu.
 */
class GUI::Menu::MainMenu: public Menu {
	/**
	 * Connect to a multiplayer game.
	 *
	 * @param window The window to use for input and rendering.
	 */
	void startMultiGame(sf::RenderWindow& window);

	/**
	 * Start a new game.
	 *
	 * @param window The window to use for input and rendering.
	 */
	void startGame(sf::RenderWindow& window);

	/**
	 * Open the settings menu.
	 *
	 * @param window The window used for input and rendering.
	 */
	void gotoSettings(sf::RenderWindow& window);

public:
	/**
	 * Constructor.
	 *
	 * @param window The window to use for input and rendering.
	 */
	MainMenu(sf::RenderWindow& window);
};

#endif

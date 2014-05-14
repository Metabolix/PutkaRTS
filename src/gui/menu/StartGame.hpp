#ifndef PUTKARTS_GUI_Menu_StartGame_HPP
#define PUTKARTS_GUI_Menu_StartGame_HPP

#include "connection/Client.hpp"

#include "gui/menu/Menu.hpp"

#include <SFML/Graphics.hpp>

namespace GUI {
	namespace Menu {
		class StartGame;
	}
}

/**
 * Class for the start game dialog.
 */
class GUI::Menu::StartGame: public Menu {
	/** Our client. */
	std::shared_ptr<Connection::Client> client;

public:
	/**
	 * @copydoc Menu::Menu
	 * @param client The client.
	 */
	StartGame(std::shared_ptr<Widget> parent, std::shared_ptr<Connection::Client> client);

	/**
	 * @copydoc Menu::updateState
	 */
	virtual void updateState(sf::RenderWindow& window);
};

#endif

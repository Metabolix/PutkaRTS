#ifndef PUTKARTS_GUI_Menu_JoinGame_HPP
#define PUTKARTS_GUI_Menu_JoinGame_HPP

#include "connection/Metaserver.hpp"

#include "gui/menu/Menu.hpp"
#include "gui/widget/List.hpp"
#include "gui/widget/Label.hpp"

#include <SFML/Graphics.hpp>

namespace GUI {
	namespace Menu {
		class JoinGame;
	}
}

/**
 * Class for the start game dialog.
 */
class GUI::Menu::JoinGame: public Menu {
	/** Connection to the metaserver. */
	Connection::Metaserver metaserver;

	/** The game list widget. */
	std::shared_ptr<GUI::Widget::List> gameList;

	/** The selected game id. */
	int selectedId;

	/** A label for the status text. */
	std::shared_ptr<GUI::Widget::Label> statusLabel;

public:
	/**
	 * @copydoc Menu::Menu
	 */
	JoinGame(std::shared_ptr<Widget> parent);

	/**
	 * @copydoc Menu::updateState
	 */
	virtual void updateState(sf::RenderWindow& window);

	/**
	 * Callback for selecting a game from the list.
	 */
	void selectGame(const GUI::Widget::List::Item& item);

	/**
	 * Join the selected game.
	 */
	void joinGame();
};

#endif

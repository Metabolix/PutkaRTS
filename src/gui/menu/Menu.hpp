#ifndef PUTKARTS_GUI_Menu_Menu_HPP
#define PUTKARTS_GUI_Menu_Menu_HPP

#include <memory>

#include "gui/graphics/TextureCache.hpp"
#include "gui/widget/Widget.hpp"
#include "gui/widget/Container.hpp"

#include <SFML/Graphics.hpp>

namespace GUI {
	namespace Menu {
		class Menu;
	}
}

/**
 * Base class for menus.
 */
class GUI::Menu::Menu: public Widget::Container {
	/** The view to use for rendering. */
	sf::View view;

	/** Pointer to parent widget. */
	std::shared_ptr<Widget> parent;

	/** Menu top logo. */
	sf::Sprite logoSprite;

	/** Keeps track whether the menu is active or not. */
	bool menuIsOpen;

protected:
	/** Menu textures. */
	static TextureCache textures;

	/**
	 * Set parent and flag the menu as open.
	 *
	 * @param parent Pointer to the parent object. Control is returned to parent when menu is closed.
	 */
	void openMenu(std::shared_ptr<Widget> parent = std::shared_ptr<Widget>());

	/**
	 * Return control to parent if one is specified and flag the menu as closed.
	 */
	void closeMenu();

public:
	/**
	 * Constructor.
	 *
	 * @param parent Pointer to the parent object. Control is returned to parent when menu is closed.
	 */
	Menu(std::shared_ptr<Widget> parent = std::shared_ptr<Widget>());

	/**
	 * Draw the widget.
	 *
	 * @param window The window to draw to.
	 */
	void draw(sf::RenderWindow& window);

	/**
	 * Is the menu open?
	 *
	 * @return Returns true if the menu is flagged as open.
	 */
	bool isOpen() const {
		return menuIsOpen;
	}
};

#endif

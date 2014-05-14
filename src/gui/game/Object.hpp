#ifndef PUTKARTS_GUI_Game_Object_HPP
#define PUTKARTS_GUI_Game_Object_HPP

#include <memory>

#include <SFML/Graphics.hpp>

namespace GUI {
	namespace Game {
		class Object;
	}
}

namespace Game {
	class Client;
	class Object;
}

/**
 * GUI class that wraps a game object and handles drawing it.
 */
class GUI::Game::Object {
	/** The real object in the game. */
	std::shared_ptr<const ::Game::Object> object;

public:
	/**
	 * Constructor.
	 *
	 * @param object The real object in the game.
	 */
	Object(std::shared_ptr<const ::Game::Object> object);

	/* Access the object. */
	const std::shared_ptr<const ::Game::Object>& getObject() const {
		return object;
	}

	/**
	 * Draw the object.
	 *
	 * @param window The window to use for rendering.
	 * @param viewer The current player.
	 * @param selected Is this object selected?
	 */
	void draw(sf::RenderWindow& window, std::shared_ptr<const ::Game::Client> viewer, bool selected = false);
};

#endif

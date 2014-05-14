#ifndef PUTKARTS_GUI_Game_MouseTracker_HPP
#define PUTKARTS_GUI_Game_MouseTracker_HPP

#include "util/Vector2.hpp"

#include <SFML/Graphics.hpp>

namespace GUI {
	namespace Game {
		class MouseTracker;
	}
}

/**
 * Class for keeping track of mouse state.
 */
class GUI::Game::MouseTracker {
	/** Screen coordinates. */
	unsigned int x, y;

	/** Game coordinates. */
	Vector2<SIUnit::Position> position;

public:
	/**
	 * Update the mouse state.
	 *
	 * @param window The window.
	 * @param view The game view.
	 * @param x The x coordinate.
	 * @param y The y coordinate.
	 */
	void update(const sf::RenderWindow& window, const sf::View& view, unsigned int x, unsigned int y);

	/**
	 * Update the mouse state using the stored coordinates.
	 *
	 * @param window The window.
	 * @param view The game view.
	 */
	void update(const sf::RenderWindow& window, const sf::View& view);

	/**
	 * Get the position in game coordinates.
	 */
	const Vector2<SIUnit::Position>& getPosition() const {
		return position;
	}
};

#endif

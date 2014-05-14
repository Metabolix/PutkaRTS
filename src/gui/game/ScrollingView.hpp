#ifndef PUTKARTS_GUI_Game_ScrollingView_HPP
#define PUTKARTS_GUI_Game_ScrollingView_HPP

#include <SFML/Graphics.hpp>

namespace GUI {
	namespace Game {
		class ScrollingView;
	}
}

/**
 * Class for scrolling.
 */
class GUI::Game::ScrollingView: public sf::View {
	/** Size of the scrollable area. */
	const sf::Vector2f contentSize;

	/** The default zoom level. */
	const std::size_t pixelsPerUnit;

	/** Is map being dragged with mouse? */
	bool mouseDragging;

	/** Window coordinates for drag origin */
	sf::Vector2i dragOrigin;

public:
	/**
	 * Constructor.
	 *
	 * @param window The window to use for view data.
	 * @param contentSize Size of the scrollable area.
	 * @param pixelsPerUnit How big is one unit in pixels?
	 */
	ScrollingView(sf::RenderWindow& window, sf::Vector2f contentSize, std::size_t pixelsPerUnit);

	/**
	 * Reset view used for map rendering
	 *
	 * @param window The window to use for view data.
	 */
	void reset(sf::RenderWindow& window);

	/**
	 * Handle map scrolling calculations.
	 *
	 * @param window The window to use for input and rendering.
	 */
	void update(sf::RenderWindow& window);
};

#endif

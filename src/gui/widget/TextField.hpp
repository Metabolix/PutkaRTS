#ifndef PUTKARTS_GUI_Widget_TextField_HPP
#define PUTKARTS_GUI_Widget_TextField_HPP

#include <string>

#include "gui/widget/Widget.hpp"

#include "SFML/Graphics.hpp"

namespace GUI {
	namespace Widget {
		class TextField;
	}
}

/**
 * One row textfield.
 */
class GUI::Widget::TextField: public Widget {
	/** The text */
	sf::Text text;

	/** Cursor index */
	unsigned int cursorIndex;

	/**
	 * Get size of font.
	 *
	 * @return Font size
	 */
	unsigned int fontSize() const;

	/**
	 * Get width of borders.
	 *
	 * @return Border size
	 */
	unsigned int borderSize() const;

	/**
	 * Get character size.
	 *
	 * @return Character size
	 */
	sf::Vector2f characterSize() const;

	/**
	 * Get start position of text on rendering.
	 *
	 * @return Text start position
	 */
	sf::Vector2f textStartPos() const;

	/**
	 * Get maximum text width text can have on rendering.
	 *
	 * @return Maximum text width
	 */
	float maxTextWidth() const;

	/**
	 * Get text width on rendering.
	 *
	 * @param str Text
	 * @return Text width
	 */
	float textWidth(const sf::String& str) const;

	/**
	 * Get text index from X coordinate.
	 *
	 * @param xPosition X coordinate
	 * @return Text index
	 */
	unsigned int getIndex(const float& xPosition) const;
public:

	/**
	 * Create Text field.
	 *
	 * @param str Text to set
	 * @param x X coordinate
	 * @param y Y coordinate
	 * @param width Width
	 * @param height Height
	 */
	TextField(const std::string& str, float x, float y, float width, float height);

	/**
	 * Set text.
	 *
	 * @param str Text to set.
	 */
	void setText(const std::string& str);

	/**
	 * Get text.
	 *
	 * @return Text
	 */
	std::string getText() const;

	/**
	 * Handles event.
	 *
	 * @param e The event
	 * @param window Window where event happened.
	 * @return True if event is text modifying or cursor moving event.
	 */
	virtual bool handleEvent(const sf::Event& e, const sf::RenderWindow& window);

	/**
	 * Draw text field.
	 *
	 * @param window The window for drawing
	 */
	virtual void draw(sf::RenderWindow& window);
};

#endif

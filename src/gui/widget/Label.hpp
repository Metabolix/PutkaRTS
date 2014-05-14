#ifndef PUTKARTS_GUI_Widget_Label_HPP
#define PUTKARTS_GUI_Widget_Label_HPP

#include "Widget.hpp"

namespace GUI {
	namespace Widget {
		class Label;
	}
}

/**
 * Class for the Label widget.
 */
class GUI::Widget::Label: public Widget {
	/** Text for the label. */
	sf::Text label;

	/** Is the label centered? */
	bool isCentered;

public:
	/**
	 * Create a Label.
	 *
	 * @param text Text to render.
	 * @param x X coordinate of the text.
	 * @param y Y coordinate of the text.
	 * @param height Height of the text.
	 */
	Label(const std::string& text = "", float x = 0, float y = 0, float height = 8);

	/**
	 * Create a Label.
	 *
	 * @param text Text to render.
	 * @param x X coordinate of the text.
	 * @param y Y coordinate of the text.
	 * @param width Maximum width of the text.
	 * @param height Maximum height of the text.
	 */
	Label(const std::string& text, float x, float y, float width, float height);

	/**
	 * Set the text.
	 *
	 * @param text The text.
	 */
	virtual void setText(const std::string& text);

	/**
	 * Set whether the text is centered or aligned to left.
	 *
	 * @param isCentered_ Is the text centered?
	 */
	virtual void setCentered(bool isCentered_) {
		isCentered = isCentered_;
	}

	/**
	 * Handle events.
	 *
	 * @param e The event.
	 * @param window The window of the event.
	 * @return Always false.
	 */
	virtual bool handleEvent(const sf::Event& e, const sf::RenderWindow& window) {
		return false;
	}

	/**
	 * Draw the Label.
	 *
	 * @param window The render window.
	 */
	virtual void draw(sf::RenderWindow& window);
};

#endif

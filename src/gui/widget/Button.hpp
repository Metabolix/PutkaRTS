#ifndef PUTKARTS_GUI_Widget_Button_HPP
#define PUTKARTS_GUI_Widget_Button_HPP

#include <functional>

#include "Widget.hpp"
#include "Label.hpp"

namespace GUI {
	namespace Widget {
		class Button;
	}
}

/**
 * Class for the button widget.
 */
class GUI::Widget::Button: public Widget {
public:
	/** Callback function type. */
	typedef std::function<void()> CallbackType;

private:
	/**
	 * Label for the button.
	 */
	Label label;

	/**
	 * Callback function for the button.
	 */
	CallbackType action;

public:
	/**
	 * Create a button with label and position.
	 *
	 * @param text Text is rendered on the button.
	 * @param x X coordinate of the button.
	 * @param y Y coordinate of the button.
	 * @param width Width of the button.
	 * @param height Height of the button.
	 * @param callback The action to take when clicked.
	 */
	Button(const std::string& text, float x, float y, float width, float height, CallbackType callback);

	/**
	 * Handle events, namely mouse clicks.
	 *
	 * @param e The event.
	 * @param window The window of the event.
	 * @return True, if this button handled the event.
	 */
	virtual bool handleEvent(const sf::Event& e, const sf::RenderWindow& window);

	/**
	 * Draw the button.
	 *
	 * @param window The render window.
	 */
	virtual void draw(sf::RenderWindow& window) {
		draw(window, false);
	};

	/**
	 * Draw the button.
	 *
	 * @param window The render window.
	 * @param highlight If true. Different background color is used.
	 */
	virtual void draw(sf::RenderWindow& window, bool highlight);
};

#endif

#ifndef PUTKARTS_GUI_Widget_Checkbox_HPP
#define PUTKARTS_GUI_Widget_Checkbox_HPP

#include <functional>

#include "Widget.hpp"

namespace GUI {
	namespace Widget {
		class Checkbox;
	}
}

/**
 * Class for the checkbox widget.
 */
class GUI::Widget::Checkbox: public Widget {
public:
	/** Callback function type. */
	typedef std::function<void(bool)> CallbackType;

private:
	/**
	 * Callback function for the checkbox.
	 */
	CallbackType action;

	/**
	 * Current state of the checkbox.
	 */
	bool isChecked;

public:
	/**
	 * Create a checkbox.
	 *
	 * @param x X coordinate of the checkbox.
	 * @param y Y coordinate of the checkbox.
	 * @param width Width of the checkbox.
	 * @param height Height of the checkbox.
	 * @param callback The action to take when clicked.
	 * @param initialState Initial state of the checkbox.
	 */
	Checkbox(float x, float y, float width, float height, bool initialState = false, CallbackType callback = NULL);

	/**
	 * Handle events, namely mouse clicks.
	 *
	 * @param e The event.
	 * @param window The window of the event.
	 * @return True, if this checkbox handled the event.
	 */
	virtual bool handleEvent(const sf::Event& e, const sf::RenderWindow& window);

	/**
	 * Draw the checkbox.
	 *
	 * @param window The render window.
	 */
	virtual void draw(sf::RenderWindow& window);

	/**
	 * Set the state of the checkbox.
	 *
	 * @param newState New state for the checkbox.
	 */
	virtual void setState(bool newState);

	/**
	 * Get the state of the checkbox.
	 *
	 * @return Returns the current state of the checkbox.
	 */
	virtual bool getState() const;
};

#endif

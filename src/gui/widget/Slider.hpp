#ifndef PUTKARTS_GUI_Widget_Slider_HPP
#define PUTKARTS_GUI_Widget_Slider_HPP

#include <functional>

#include "Widget.hpp"

namespace GUI {
	namespace Widget {
		class Slider;
	}
}

/**
 * Class for the slider widget.
 */
class GUI::Widget::Slider: public Widget {
public:
	/** Callback function type. */
	typedef std::function<void(float)> CallbackType;

private:
	/** Callback function for the button. */
	CallbackType action;

	float sliderPosition;
	float rangeMax;
	float rangeMin;
	bool vertical;
	bool isDragged;
	float oldMouseCoordinate;

	/**
	 * Calculate the slider length.
	 */
	float getSliderLength() const;

public:
	/**
	 * Create a button with label and position.
	 *
	 * @param x X coordinate of the slider.
	 * @param y Y coordinate of the slider.
	 * @param length Length of the slider
	 * @param thickness Thickness of the slider.
	 * @param vertical Orientation of the slider. Horizontal by default.
	 * @param callback The action to take when moved.
	 * @param rangeMin Low end of the desired value range.
	 * @param rangeMax High end of the desired value range.
	 * @param initialValue Initial value for the slider.
	 */
	Slider(float x, float y, float length, float thickness, bool vertical = false, CallbackType callback = NULL, float rangeMin = 0.0f, float rangeMax = 1.0f, float initialValue = 0.0f);

	/**
	 * Handle events.
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
	virtual void draw(sf::RenderWindow& window);

	/**
	 * Set the slider position.
	 *
	 * @param v New position of the slider.
	 */
	virtual void setScrollPosition(float v);

	/**
	 * Fet the current slider position.
	 *
	 * @return Returns the current slider position.
	 */
	virtual float getScrollPosition() const;

	/**
	 * Transform raw value from a slider into a value within given range.
	 *
	 * @param raw The raw value from the slider.
	 * @param min Low end of the desired value range.
	 * @param max High end of the desired value range.
	 * @return Returns the transformed value. (min + raw * (max - min))
	 */
	static float transformValue(float raw, float min, float max);

	/**
	 * Inverse of the transformValue(). Transforms the given value into range used by sliders.
	 *
	 * @param value The value to transform.
	 * @param min Low end of the desired value range.
	 * @param max High end of the desired value range.
	 * @return Returns the transformed value. ((value - min) / (max - min))
	 */
	static float inverseTransformValue(float value, float min, float max);
};

#endif

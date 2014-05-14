#ifndef PUTKARTS_GUI_Widget_Container_HPP
#define PUTKARTS_GUI_Widget_Container_HPP

#include <list>
#include <memory>

#include "Widget.hpp"

namespace GUI {
	namespace Widget {
		class Container;
	}
}

/**
 * Base class for widget containers.
 */
class GUI::Widget::Container: public Widget {
protected:
	/** The list type to hold object pointers. */
	typedef std::list<std::shared_ptr<Widget> > WidgetListType;

	/** List of the objects in this container. */
	WidgetListType objects;
public:
	/** Destructor. */
	virtual ~Container();

	/**
	 * Insert another object.
	 *
	 * @param object The object to insert.
	 */
	virtual void insert(const std::shared_ptr<Widget>& object);

	/**
	 * Erase a previously added object.
	 *
	 * @param object The object to insert.
	 */
	virtual void erase(const std::shared_ptr<Widget>& object);

	/**
	 * Insert another object.
	 *
	 * This version automatically wraps the pointer in a shared_ptr.
	 *
	 * @param object The object to insert.
	 */
	void insert(Widget* object) {
		return insert(std::shared_ptr<Widget>(object));
	}

	/**
	 * Draw the objects in this container.
	 */
	virtual void draw(sf::RenderWindow& window);

	/**
	 * Handle an event.
	 *
	 * This version passes the event to each child object until one returns true.
	 *
	 * @param window The window of the event.
	 * @param event The event to handle.
	 * @return true, if one of the children handled the event. Otherwise false.
	 */
	virtual bool handleEvent(const sf::Event& event, const sf::RenderWindow& window);

	/**
	 * Update the objects in this container.
	 *
	 * @param window The current window.
	 */
	virtual void updateState(sf::RenderWindow& window);

	/**
	 * Move widget to the front of the list. First widget receives events first and is drawn last.
	 *
	 * @param widget Pointer ot the widget to be moved. Used only to identify the widget.
	 */
	virtual void activate(const Widget* widget);

	/**
	 * Erase all objects.
	 */
	virtual void clear();
};

#endif

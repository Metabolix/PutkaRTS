#ifndef PUTKARTS_Game_Task_HPP
#define PUTKARTS_Game_Task_HPP

#include <list>
#include <memory>

namespace Game {
	class Task;
	class Object;
	class ObjectAction;
}

/**
 * This class describes a task that some units are performing.
 */
struct Game::Task {
	/** The action. */
	std::shared_ptr<const ObjectAction> action;

	/** The actor objects. */
	std::list<std::weak_ptr<Object> > actors;

	/** The target objects. */
	std::list<std::weak_ptr<Object> > targets;

	/** A dummy target. This is used if there aren't any real targets, e.g. when moving to a location. */
	std::shared_ptr<Object> dummy;
};

#endif

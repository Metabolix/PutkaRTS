#ifndef PUTKARTS_Game_ObjectAction_HPP
#define PUTKARTS_Game_ObjectAction_HPP

#include <string>
#include <memory>

namespace Game {
	class Game;
	class Object;
	class ObjectAction;
}

/**
 * This class describes object action.
 */
class Game::ObjectAction {
public:
	typedef std::string IdType;

	/** Predefined action "delete". */
	static const IdType DELETE;

	/** Predefined action "move". */
	static const IdType MOVE;

	/** An unique identifier for this action. */
	IdType id;

	/** The name to tell the users. */
	std::string name;

	/**
	 * Run object action.
	 *
	 * @param actor Acting object.
	 * @param target Target of the action.
	 * @return Whether it was possible to run the action.
	 */
	bool runAction(std::shared_ptr<Object> actor, std::shared_ptr<Object> target) {
		// TODO: Run the action.
		return false;
	}
};

#endif

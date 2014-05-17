#ifndef PUTKARTS_Game_ObjectType_HPP
#define PUTKARTS_Game_ObjectType_HPP

#include <string>

#include "util/Scalar.hpp"

namespace Game {
	class ObjectType;
}

/**
 * This class describes object type.
 */
class Game::ObjectType {
public:
	typedef std::string IdType;

	/** An unique identifier for this type. */
	IdType id;

	/** The name to tell the users. */
	std::string name;

	/** Does this kind of unit ever change? */
	bool immutable;

	/** The size. */
	Scalar<SIUnit::Length> radius;

	/** The maximum velocity. */
	Scalar<SIUnit::Velocity> maxVelocity;

	/** The line of sight. */
	Scalar<SIUnit::Length> lineOfSight;

	/** Maximum hit points. */
	int maxHitPoints;
};

#endif

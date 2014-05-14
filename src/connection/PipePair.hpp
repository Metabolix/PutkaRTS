#ifndef PUTKARTS_Connection_PipePair_HPP
#define PUTKARTS_Connection_PipePair_HPP

#include "EndPoint.hpp"

namespace Connection {
	class PipePair;
}

/**
 * Two-ended local connection.
 */
class Connection::PipePair {
	class Pipe;
	class Mixer;

	std::shared_ptr<EndPoint> end1, end2;

public:
	/**
	 * Construct a pair of pipes.
	 */
	PipePair();

	/**
	 * Get one end of the pipe pair.
	 */
	std::shared_ptr<EndPoint> getEnd1() {
		return end1;
	}

	/**
	 * Get another end of the pipe pair.
	 */
	std::shared_ptr<EndPoint> getEnd2() {
		return end2;
	}
};

#endif

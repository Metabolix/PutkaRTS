/*
 * Two-ended connection implemented as a local pipe.
 *
 * Copyright 2011 Lauri Kenttä
 *
 * This file is part of PutkaRTS.
 *
 * PutkaRTS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PutkaRTS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PutkaRTS.  If not, see <http://www.gnu.org/licenses/>.
 */

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

	boost::shared_ptr<EndPoint> end1, end2;

public:
	/**
	 * Construct a pair of pipes.
	 */
	PipePair();

	/**
	 * Get one end of the pipe pair.
	 */
	boost::shared_ptr<EndPoint> getEnd1() {
		return end1;
	}

	/**
	 * Get another end of the pipe pair.
	 */
	boost::shared_ptr<EndPoint> getEnd2() {
		return end2;
	}
};

#endif

/*
 * Class for keeping track of time.
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

#ifndef PUTKARTS_Clock_HPP
#define PUTKARTS_Clock_HPP

#include <memory>

#include "Scalar.hpp"

/**
 * Class for handling time.
 */
class Clock {
	/** Class for the actual implementation. */
	struct ClockImpl;

	/** Pointer to the actual clock. */
	std::auto_ptr<const ClockImpl> impl;

public:
	/**
	 * Constructor.
	 *
	 * @param time The initial time.
	 */
	Clock(Scalar<SIUnit::Time> time = 0);

	/**
	 * Get the current time.
	 *
	 * @return The current time.
	 */
	Scalar<SIUnit::Time> getTime() const;

	/**
	 * Reset the clock.
	 *
	 * @param time The new time.
	 */
	void reset(Scalar<SIUnit::Time> time = 0);

	/**
	 * Pause the clock.
	 */
	void pause();

	/**
	 * Unpause the clock.
	 */
	void unpause();
};

#endif

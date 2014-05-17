#ifndef PUTKARTS_Clock_HPP
#define PUTKARTS_Clock_HPP

#include <memory>

#include "Scalar.hpp"

/**
 * Class for handling time.
 */
class Clock {
	/** Class for the actual implementation. */
	class ClockImpl;

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

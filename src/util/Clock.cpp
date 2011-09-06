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

#include "Clock.hpp"
#include <boost/chrono.hpp>

/**
 * A clock implementation using boost::chrono.
 */
struct Clock::ClockImpl {
	/** Is this clock running? */
	const bool running;

	/** The time when this clock was started. */
	const Scalar<SIUnit::Time> time0;

	/** The real time when the clock was started. */
	const boost::chrono::high_resolution_clock::time_point real0;

	/**
	 * Constructor.
	 *
	 * @param time0_ The current time.
	 * @param running_ Is this clock runnint?
	 */
	ClockImpl(Scalar<SIUnit::Time> time0_, bool running_ = true):
		running(running_),
		time0(time0_),
		real0(boost::chrono::high_resolution_clock::now()) {
	}

	/**
	 * Get the current time.
	 *
	 * @return The current time.
	 */
	Scalar<SIUnit::Time> getTime() const {
		if (!running) {
			return time0;
		}
		boost::chrono::high_resolution_clock::time_point real1(boost::chrono::high_resolution_clock::now());
		boost::chrono::duration<double> dt(real1 - real0);
		return time0 + Scalar<SIUnit::Time>(dt.count());
	}
};

Clock::Clock(Scalar<SIUnit::Time> time):
	impl(new ClockImpl(time)) {
}

Scalar<SIUnit::Time> Clock::getTime() const {
	return impl->getTime();
}

void Clock::reset(Scalar<SIUnit::Time> time) {
	impl.reset(new ClockImpl(time));
}

void Clock::pause() {
	if (impl->running) {
		impl.reset(new ClockImpl(impl->getTime(), false));
	}
}

void Clock::unpause() {
	if (!impl->running) {
		impl.reset(new ClockImpl(impl->getTime(), true));
	}
}

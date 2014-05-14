#include <chrono>

#include "Clock.hpp"

/**
 * A clock implementation using std::chrono.
 */
struct Clock::ClockImpl {
	/** Is this clock running? */
	const bool running;

	/** The time when this clock was started. */
	const Scalar<SIUnit::Time> time0;

	/** The real time when the clock was started. */
	const std::chrono::high_resolution_clock::time_point real0;

	/**
	 * Constructor.
	 *
	 * @param time0_ The current time.
	 * @param running_ Is this clock runnint?
	 */
	ClockImpl(Scalar<SIUnit::Time> time0_, bool running_ = true):
		running(running_),
		time0(time0_),
		real0(std::chrono::high_resolution_clock::now()) {
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
		std::chrono::high_resolution_clock::time_point real1(std::chrono::high_resolution_clock::now());
		std::chrono::duration<double> dt(real1 - real0);
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

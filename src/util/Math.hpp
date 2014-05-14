#ifndef PUTKARTS_Math_HPP
#define PUTKARTS_Math_HPP

/**
 * Mathematical helper functions.
 */
namespace Math {
	/**
	 * The value of pi.
	 */
	static const double pi = 3.1415926535897932384626433832795028841971693993751;

	/**
	 * Convert radians to degrees.
	 */
	inline double toDegrees(double rad) {
		return 180 / pi * rad;
	}

	/**
	 * Convert radians to degrees.
	 */
	inline double toRadians(double deg) {
		return pi / 180 * deg;
	}
}

#endif

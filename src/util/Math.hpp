/*
 * Mathematical helper functions.
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

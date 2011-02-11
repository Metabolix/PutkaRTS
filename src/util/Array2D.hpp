/*
 * Class for easy allocation of 2D arrays.
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

#ifndef PUTKARTS_Array2D_HPP
#define PUTKARTS_Array2D_HPP

#include <vector>
#include <stdexcept>
#include <boost/format.hpp>

/**
 * Class for two-dimensional arrays with safe dynamic allocation and bounds checking.
 */
template <typename T>
class Array2D {
	/** Size type. */
	typedef typename std::vector<T>::size_type SizeType;

	/** Array size in x direction. */
	SizeType sizeX;

	/** Array size in y direction. */
	SizeType sizeY;

	/** Vector for the data. */
	std::vector<T> data;
public:
	/**
	 * Construct with the given size.
	 *
	 * @param initSizeX The initial size in x direction.
	 * @param initSizeY The initial size in y direction.
	 * @param value The value to use for initializing the array.
	 */
	Array2D(SizeType initSizeX = 0, SizeType initSizeY = 0, const T& value = T()):
		sizeX(initSizeX),
		sizeY(initSizeY),
		data(initSizeX * initSizeY, value) {
	}

	/**
	 * Clear and resize to the given size.
	 *
	 * @param newSizeX The new size in x direction.
	 * @param newSizeY The new size in y direction.
	 * @param value The value to use for initializing the new array.
	 */
	void resize(SizeType newSizeX = 0, SizeType newSizeY = 0, const T& value = T()) {
		data.clear();
		sizeX = 0;
		sizeY = 0;

		data.resize(newSizeX * newSizeY, value);
		sizeX = newSizeX;
		sizeY = newSizeY;
	}

	/**
	 * Get x size.
	 */
	SizeType getSizeX() const {
		return sizeX;
	}

	/**
	 * Get y size.
	 */
	SizeType getSizeY() const {
		return sizeY;
	}

	/**
	 * Get reference to a value.
	 */
	T& operator() (SizeType x, SizeType y) {
		if (x < 0 || x >= sizeX || y < 0 || y >= sizeY) {
			throw std::out_of_range((boost::format("Index out of range (%d, %d)!") % x % y).str());
		}
		return data[x + y * sizeX];
	}

	/**
	 * Get reference to a value; const version.
	 */
	const T& operator() (SizeType x, SizeType y) const {
		if (x < 0 || x >= sizeX || y < 0 || y >= sizeY) {
			throw std::out_of_range((boost::format("Index out of range (%d, %d)!") % x % y).str());
		}
		return data[x + y * sizeX];
	}
};

#endif

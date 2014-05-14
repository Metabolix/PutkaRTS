#ifndef PUTKARTS_Array2D_HPP
#define PUTKARTS_Array2D_HPP

#include <vector>
#include <stdexcept>
#include <algorithm>
#include <boost/format.hpp>

/**
 * Class for two-dimensional arrays with safe dynamic allocation and bounds checking.
 */
template <typename T>
class Array2D {
public:
	/** Size type. */
	typedef typename std::vector<T>::size_type SizeType;

private:
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
	 * Swap this and another array.
	 *
	 * @param other The other array.
	 */
	void swap(Array2D<T>& other) {
		std::swap(sizeX, other.sizeX);
		std::swap(sizeY, other.sizeY);
		std::swap(data, other.data);
	}

	/**
	 * Is the array empty?
	 */
	bool empty() {
		return data.empty();
	}

	/**
	 * Clear the array (resize to 0x0).
	 */
	void clear() {
		data.clear();
		sizeX = 0;
		sizeY = 0;
	}

	/**
	 * Clear and resize to the given size.
	 *
	 * @param newSizeX The new size in x direction.
	 * @param newSizeY The new size in y direction.
	 * @param value The value to use for initializing the new array.
	 */
	void resize(SizeType newSizeX = 0, SizeType newSizeY = 0, const T& value = T()) {
		if (sizeX == newSizeX) {
			data.resize(sizeX * newSizeY, value);
			sizeY = newSizeY;
			return;
		}
		Array2D<T> tmp(newSizeX, newSizeY, value);
		for (SizeType y = 0; y < std::min(newSizeY, sizeY); ++y) {
			for (SizeType x = 0; x < std::min(newSizeX, sizeX); ++x) {
				tmp(x, y) = operator()(x, y);
			}
		}
		swap(tmp);
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
		// (x < 0 || y < 0) == false, because SizeType is unsigned.
		if (x >= sizeX || y >= sizeY) {
			throw std::out_of_range((boost::format("Index out of range (%d, %d)!") % x % y).str());
		}
		return data[x + y * sizeX];
	}

	/**
	 * Get reference to a value; const version.
	 */
	const T& operator() (SizeType x, SizeType y) const {
		// (x < 0 || y < 0) == false, because SizeType is unsigned.
		if (x >= sizeX || y >= sizeY) {
			throw std::out_of_range((boost::format("Index out of range (%d, %d)!") % x % y).str());
		}
		return data[x + y * sizeX];
	}
};

#endif

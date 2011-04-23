/*
 * Functions for deserializing data.
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

#ifndef PUTKARTS_Deserializer_HPP
#define PUTKARTS_Deserializer_HPP

#include "Scalar.hpp"
#include "Vector2.hpp"

#include <sstream>
#include <string>

/**
 * Class for deserializing data.
 */
class Deserializer {
	/** The data. */
	std::istringstream data;

public:
	/**
	 * Construct a new deserializer.
	 */
	Deserializer(const std::string& data_):
		data(data_) {
	}

	/**
	 * Deserialize an unsigned integer.
	 *
	 * @param value Reference to the object that shall hold the value.
	 */
	void get(unsigned int& value);

	/**
	 * Deserialize a Scalar<> object.
	 *
	 * @param value Reference to the object that shall hold the value.
	 */
	void get(Scalar<>& value);

	/**
	 * Deserialize any kind of Scalar<T> object.
	 *
	 * @param value Reference to the object that shall hold the value.
	 */
	template <typename T>
	inline void get(Scalar<T>& value) {
		Scalar<> tmp;
		get(tmp);
		value.unsafeAssign(tmp);
	}

	/**
	 * Deserialize any kind of Vector<T> object.
	 *
	 * @param value Reference to the object that shall hold the value.
	 */
	template <typename T>
	inline void get(Vector2<T>& value) {
		get(value.x);
		get(value.y);
	}
};

#endif

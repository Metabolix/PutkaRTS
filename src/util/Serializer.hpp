/*
 * Functions for serializing and deserializing data.
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

#ifndef PUTKARTS_Serializer_HPP
#define PUTKARTS_Serializer_HPP

#include <sstream>
#include <string>

#include "Scalar.hpp"
#include "Vector2.hpp"

/**
 * Functions for serializing and unserializing data.
 */
class Serializer {
	/** The data. */
	std::ostringstream data;

public:
	/**
	 * Get the serialized data.
	 *
	 * @return The data.
	 */
	std::string getData() const {
		return data.str();
	}

	/**
	 * Serialize an unsigned integer.
	 *
	 * @param value The value to serialize.
	 */
	void put(const unsigned int& value);

	/**
	 * Serialize an integer.
	 *
	 * @param value The value to serialize.
	 */
	void put(const int& value);

	/**
	 * Serialize a boolean.
	 *
	 * @param value The value to serialize.
	 */
	void put(const bool& value);

	/**
	 * Serialize a Scalar<> object.
	 *
	 * @param value The value to serialize.
	 */
	void put(const Scalar<>& value);

	/**
	 * Serialize a string.
	 *
	 * @param value The value to serialize.
	 */
	void put(const std::string& value);

	/**
	 * Serialize any kind of Scalar<T>.
	 *
	 * @param value The value to serialize.
	 */
	template <typename T>
	inline void put(const Scalar<T>& value) {
		put(value.strip());
	}

	/**
	 * Serialize any kind of Vector2<T>.
	 *
	 * @param value The value to serialize.
	 */
	template <typename T>
	inline void put(const Vector2<T>& value) {
		put(value.x);
		put(value.y);
	}
};

#endif

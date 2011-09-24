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

#include "Serializer.hpp"

void Serializer::put(const unsigned int& value) {
	data << value << '\n';
}

void Serializer::put(const int& value) {
	data << value << '\n';
}

void Serializer::put(const bool& value) {
	data << (value ? 1 : 0) << '\n';
}

void Serializer::put(const Scalar<>& value) {
	data << value.getDouble() << '\n';
}

void Serializer::put(const std::string& value) {
	unsigned int size = value.size();
	put(size);
	data.write(value.c_str(), size);
	data << '\n';
}

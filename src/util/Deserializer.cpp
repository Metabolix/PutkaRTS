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

#include "Deserializer.hpp"

#include <stdexcept>
#include <vector>

static void finishValue(std::istream& data) {
	if (!data || data.get() != '\n') {
		throw std::runtime_error("Invalid data (missing '\n')!");
	}
}

void Deserializer::get(unsigned int& value) {
	data >> value;
	finishValue(data);
}

void Deserializer::get(Scalar<>& value) {
	data >> value.value;
	finishValue(data);
}

void Deserializer::get(std::string& value) {
	std::vector<char> ret;
	unsigned int size;
	get(size);
	ret.resize(size);
	data.read(&ret[0], size);
	finishValue(data);
	value.assign(ret.begin(), ret.end());
}

/*
 * Game object class implementation.
 *
 * Copyright 2011 Petri Österman
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

#include "TechTree.hpp"

#include <fstream>

void World::TechTree::load(const std::string& directory) {
	// TODO: Load ObjectTypes and ObjectActions from tech tree file.

	// Generate one object type for testing.
	std::ifstream stream;
	std::string objectTypeName = "dummy";
	boost::shared_ptr<World::ObjectType> testObjectType(new World::ObjectType(stream));
	objectTypes.insert(std::make_pair(objectTypeName, testObjectType));
}

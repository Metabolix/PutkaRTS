/*
 * Map loading system.
 *
 * Copyright 2011 Leo Lehikoinen
 * Copyright 2011 Lauri Kenttä
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

#include "util/Path.hpp"

#include "Game.hpp"
#include "Map.hpp"
#include "Object.hpp"

#include <stdexcept>
#include <fstream>
#include <sstream>
#include <boost/algorithm/string.hpp>

void Map::loadInfo(std::istream& file) {
	std::string line;
	std::stringstream buffer;
	while (std::getline(file, line)) {
		boost::trim(line);
		if (line.empty()) {
			break;
		}
		buffer << line << std::endl;
	}
	info.clear();
	info.load(buffer);
}

void Map::loadTileInfo(std::istream& file) {
	tileInfoMap.clear();
	std::string line;
	while (std::getline(file, line)) {
		boost::trim(line);
		if (line.empty()) {
			break;
		}

		std::istringstream buffer(line);
		char tile;
		TileInfo info;
		if (!(buffer >> tile >> info.ground >> info.water >> info.texture)) {
			throw std::runtime_error("definition section malformed!");
		}
		tileInfoMap[tile] = info;
	}
	if (tileInfoMap.empty()) {
		throw std::runtime_error("zero dimensions!");
	}
}

void Map::loadTileMap(std::istream& file) {
	tileMap.clear();
	std::string line;
	while (getline(file, line)) {
		boost::trim(line);
		if (line.empty()) {
			break;
		}
		if (tileMap.getSizeX() && tileMap.getSizeX() != line.size()) {
			throw std::runtime_error("width is not constant!");
		}
		const SizeType y = tileMap.getSizeY();
		tileMap.resize(line.size(), y + 1);
		for (SizeType x = 0; x < tileMap.getSizeX(); ++x) {
			if (tileInfoMap.find(line[x]) == tileInfoMap.end()) {
				throw std::runtime_error(std::string() + "tile " + line[x] + " used but not defined!");
			}
			tileMap(x, y) = line[x];
		}
	}
	if (tileMap.empty()) {
		throw std::runtime_error("zero dimensions!");
	}
}

void Map::load(const std::string& directory_)
try {
	directory = directory_;
	std::string filename = Path::findDataPath(directory, "data.txt");

	std::ifstream file(filename.c_str());
	if (!file) {
		throw std::runtime_error(filename + " could not be opened!");
	}

	try {
		loadInfo(file);
		loadTileInfo(file);
		loadTileMap(file);
	} catch (std::runtime_error& e) {
		throw std::runtime_error(filename + " has invalid format: " + e.what());
	}

	if (file.bad()) {
		throw std::runtime_error(filename + " is invalid!");
	}
} catch (...) {
	info.clear();
	tileMap.clear();
	tileInfoMap.clear();
	throw;
}

void Map::createInitialObjects(Game& game) const {
	// TODO: Create starting objects from map data.

	// Create few objects and add them to game for testing.
	std::string objectTypeName = "dummy";
	Vector2<SIUnit::Position> pos(10, 10);
	for (int i = 0; i < 3; ++i) {
		boost::shared_ptr<World::Object> testObject(new World::Object(techTree->getObjectType(objectTypeName), pos));
		game.insertObject(testObject);
		pos += pos;
	}
}

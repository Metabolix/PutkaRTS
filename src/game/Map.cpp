/*
 * Map loading system.
 *
 * Copyright 2011 Leo Lehikoinen
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
 
#include "Map.hpp"

#include <stdexcept>
#include <string>
#include <map>
#include <fstream>
#include <boost/algorithm/string.hpp>
 
void Map::load(const std::string& filename) {
	std::ifstream file(filename.c_str());
	if (!file) {
		throw std::runtime_error(filename + " could not be opened!");
	}

	bool emptyLine = false;
	std::vector<std::string> mapArray;
	while (file && !emptyLine) {
		std::string line;
		if (!getline(file, line)) {
			break;
		}
		boost::trim(line);
		if (line.empty()) {
			emptyLine = true;
			break;
		}
		mapArray.push_back(line);
	}

	if (!emptyLine) {
		throw std::runtime_error(filename + " contains invalid format (no newline)!");
	}

	while (file) {
		char tile;
		TileInfo info;
		file >> tile >> info.groundTile >> info.waterTile >> info.texturePath;
		tileConfig[tile] = info;
	}

	if (mapArray.size() == 0 || mapArray[0].size() == 0) {
		throw std::runtime_error(filename + " contains invalid format (invalid (zero) dimensions)!");
	}

	tileMap.resize(mapArray[0].length(), mapArray.size());

	for (size_t y = 0; y < mapArray.size(); ++y) {
		if (mapArray[y].length() != mapArray[0].length()) {
			throw std::runtime_error(filename + " contains invalid format (width is not constant)!");
		}
		for (size_t x = 0; x < mapArray[y].length(); ++x) {
			char current = mapArray[y][x];
			if (tileConfig.find(current) == tileConfig.end()) {
				throw std::runtime_error(filename + " contains invalid format (tile " + current + " used but not defined)!");
			}
			tileMap(x, y) = current;
		}
	}
}

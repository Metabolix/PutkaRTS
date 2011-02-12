/*
 * Map class definition.
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

#ifndef PUTKARTS_Map_HPP
#define PUTKARTS_Map_HPP

#include <string>
#include <map>
#include "util/Array2D.hpp"

/**
 * Struct for tile's information
 */
struct TileInfo {
	bool groundTile, waterTile;
	std::string texturePath;
};

/**
 * This class describes the map format
 */
class Map {
	/**
	 * Map in array of tiles.
	 */
	Array2D<char> tileMap;

	/**
	 * Store each tile's info.
	 */
	std::map<char, TileInfo> tileInfoMap;
public:
	/**
	 * Default constructor.
	 */
	Map() {
		// Nothing here.
	}

	/**
	 * Constructor with the filename.
	 * 
	 * @param filename Map's filename.
	 */
	Map(const std::string& filename) {
		load(filename);
	}

	/**
	 * Load a map structure from a file
	 * 
	 * @param filename Map's filename.
	 * @throw std::runtime_error Thrown if the map can't be loaded.
	 */
	void load(const std::string& filename);

	/**
	 * Get x size.
	 */
	Array2D<char>::SizeType getSizeX() const {
		return tileMap.getSizeX();
	}

	/**
	 * Get y size.
	 */
	Array2D<char>::SizeType getSizeY() const {
		return tileMap.getSizeY();
	}

	/**
	 * Get TileInfo at location (x, y).
	 */
	const TileInfo& operator() (Array2D<char>::SizeType x, Array2D<char>::SizeType y) const {
		std::map<char, TileInfo>::const_iterator it;
		it = tileInfoMap.find(tileMap(x, y));
		return it->second;
	}
};

#endif

/*
 * Map class definition.
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

#ifndef PUTKARTS_Map_HPP
#define PUTKARTS_Map_HPP

#include <string>
#include <map>
#include <boost/shared_ptr.hpp>

#include "util/Array2D.hpp"
#include "util/Configuration.hpp"
#include "TechTree.hpp"

class Game;

/**
 * This class describes the map format
 */
class Map {
public:
	/**
	 * Type of size used in public methods
	 */
	typedef Array2D<char>::SizeType SizeType;

	/**
	 * Struct for tile's information
	 */
	struct TileInfo {
		bool ground, water;
		std::string texture;
	};

	/**
	 * Map type for storing the TileInfo structures.
	 */
	typedef std::map<char, TileInfo> TileInfoMap;
private:
	/**
	 * Store the map directory name, e.g. "maps/somemap".
	 */
	std::string directory;

	/** Map information. */
	Configuration info;

	/**
	 * Map in array of tiles.
	 */
	Array2D<char> tileMap;

	/**
	 * Store each tile's info.
	 */
	TileInfoMap tileInfoMap;

	/**
	 * Map's tech tree.
	 */
	boost::shared_ptr<World::TechTree> techTree;

	/**
	 * Load map info (name and such).
	 *
	 * @param file The input stream.
	 * @throw std::runtime_error Thrown if the info section is invalid.
	 */
	void loadInfo(std::istream& file);

	/**
	 * Load tile info.
	 *
	 * @param file The input stream.
	 * @throw std::runtime_error Thrown if the tile data is invalid.
	 */
	void loadTileInfo(std::istream& file);

	/**
	 * Load tiles.
	 *
	 * @param file The input stream.
	 * @throw std::runtime_error Thrown if the map is invalid.
	 */
	void loadTileMap(std::istream& file);
public:
	/**
	 * Default constructor.
	 */
	Map() {
		// Nothing here.
	}

	/**
	 * Constructor that also loads the map.
	 *
	 * @param directory Map's directory.
	 */
	Map(const std::string& directory) {
		load(directory);
		techTree = boost::shared_ptr<World::TechTree>(new World::TechTree("dummy"));
	}

	/**
	 * Load a map from a directory.
	 *
	 * @param directory Map's directory.
	 * @throw std::runtime_error Thrown if the map can't be loaded.
	 */
	void load(const std::string& directory);

	/**
	 * Get the map directory; necessary for loading tiles, for example.
	 */
	const std::string& getDirectory() const {
		return directory;
	}

	/**
	 * Get x size.
	 */
	SizeType getSizeX() const {
		return tileMap.getSizeX();
	}

	/**
	 * Get y size.
	 */
	SizeType getSizeY() const {
		return tileMap.getSizeY();
	}

	/**
	 * Get TileInfo at location (x, y).
	 */
	const TileInfo& operator() (SizeType x, SizeType y) const {
		return tileInfoMap.find(tileMap(x, y))->second;
	}

	/**
	 * Get the tile info map.
	 */
	const TileInfoMap& getTileInfoMap() const {
		return tileInfoMap;
	}

	/**
	 * Get map's tech tree.
	 */
	World::TechTree& getTechTree() const {
		return *techTree;
	}

	/**
	 * Create and add map's initial objects to game.
	 *
	 * @param game The game to add the objects.
	 */
	void createInitialObjects(Game& game) const;
};

#endif

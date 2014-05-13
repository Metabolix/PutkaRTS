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

#ifndef PUTKARTS_Game_Map_HPP
#define PUTKARTS_Game_Map_HPP

#include <string>
#include <map>
#include <memory>

#include "util/Array2D.hpp"
#include "lua/Lua.hpp"

namespace Game {
	class Game;
	class Map;
}

/**
 * This class describes the map format
 */
class Game::Map: protected Lua {
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

	/**
	 * Information about a player (start position).
	 */
	struct Player {
		Vector2<SIUnit::Position> startPosition;
	};

	/**
	 * Type for storing the players.
	 */
	typedef std::map<int, Player> PlayerContainerType;

private:
	/**
	 * Store the map directory name, e.g. "maps/somemap".
	 */
	std::string directory;

	/**
	 * Map in array of tiles.
	 */
	Array2D<char> tileMap;

	/**
	 * Store each tile's info.
	 */
	TileInfoMap tileInfoMap;

	/**
	 * Players (start positions) on the map.
	 */
	PlayerContainerType players;

public:
	/**
	 * Default constructor.
	 */
	Map();

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
	 * Get the players.
	 */
	const PlayerContainerType& getPlayers() const {
		return players;
	}

private:
	/**
	 * Lua callback: Set one tile info.
	 *
	 * f(string char, bool ground, bool water, string texture)
	 */
	void luaSetTileInfo();

	/**
	 * Lua callback: Set tiles.
	 *
	 * f(int x0, int y0, string tiles)
	 */
	void luaSetTileRow();

	/**
	 * Lua callback: Set player.
	 *
	 * f(int which, float startX, float startY)
	 */
	void luaSetPlayer();
};

#endif

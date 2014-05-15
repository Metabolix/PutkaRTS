#ifndef PUTKARTS_Game_Map_HPP
#define PUTKARTS_Game_Map_HPP

#include <string>
#include <map>
#include <memory>

#include "util/Array2D.hpp"
#include "util/Vector2.hpp"
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

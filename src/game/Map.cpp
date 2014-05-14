#include <sstream>
#include <functional>

#include "util/Path.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "Object.hpp"

Game::Map::Map() {
	bind("tile", std::bind(&Map::luaSetTileInfo, this));
	bind("row", std::bind(&Map::luaSetTileRow, this));
	bind("player", std::bind(&Map::luaSetPlayer, this));
}

void Game::Map::luaSetTileInfo() {
	TileInfo info;
	char tile = get<String>(1).at(0);
	info.ground = get<Boolean>(2);
	info.water = get<Boolean>(3);
	info.texture = get<String>(4);
	tileInfoMap[tile] = info;
}

void Game::Map::luaSetTileRow() {
	std::string row(get<String>(1));
	if (tileMap.empty()) {
		tileMap.resize(row.size(), 1);
	} else {
		if (row.size() != getSizeX()) {
			throw Lua::Exception("Inconsistent row length in map!");
		}
		tileMap.resize(getSizeX(), getSizeY() + 1);
	}
	SizeType y = getSizeY() - 1;
	for (SizeType x = 0; x < getSizeX(); ++x) {
		if (tileInfoMap.find(row[x]) == tileInfoMap.end()) {
			throw Lua::Exception("Invalid character: " + row.substr(x, 1));
		}
		tileMap(x, y) = row[x];
	}
}

void Game::Map::luaSetPlayer() {
	Player tmp;
	int which = get<Integer>(1);
	tmp.startPosition.x = get<Number>(2);
	tmp.startPosition.y = get<Number>(3);
	players[which] = tmp;
}

void Game::Map::load(const std::string& directory_)
try {
	directory = directory_;
	directories.clear();
	directories.push_back(directory);
	tileMap.clear();
	tileInfoMap.clear();
	runFile<void>(Path::findDataPath(directory, "map.lua"));
} catch (...) {
	directory.clear();
	tileMap.clear();
	tileInfoMap.clear();
	throw;
}

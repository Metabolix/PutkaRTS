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

#include <sstream>
#include <boost/bind.hpp>

Game::Map::Map() {
	bind("tile", boost::bind(&Map::luaSetTileInfo, this));
	bind("row", boost::bind(&Map::luaSetTileRow, this));
	bind("player", boost::bind(&Map::luaSetPlayer, this));
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

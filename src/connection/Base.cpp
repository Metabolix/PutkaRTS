/*
 * Base class for game connections.
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

#include "Base.hpp"

#include "game/Game.hpp"

void Connection::Base::initGame() {
	state = INIT;
	boost::shared_ptr<Game::Map> map(new Game::Map());
	map->load("maps/testmap");
	game.reset(new Game::Game(map));

	const Game::Game::PlayerContainerType& players(game->getPlayers());
	Game::Game::PlayerContainerType::const_iterator p = players.begin();

	for (ClientInfoContainerType::iterator i = clients.begin(); i != clients.end(); ++i) {
		game->insertClient(i->second);
		if (p != players.end()) {
			i->second->players.insert(p++->first);
		}
	}
	while (p != players.end()) {
		clients.begin()->second->players.insert(p++->first);
	}
}

void Connection::Base::startGame() {
	state = PLAY;
}

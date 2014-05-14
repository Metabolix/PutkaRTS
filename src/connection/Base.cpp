#include "Base.hpp"

#include "game/Game.hpp"

void Connection::Base::initGame() {
	state = INIT;
	std::shared_ptr<Game::Map> map(new Game::Map());
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

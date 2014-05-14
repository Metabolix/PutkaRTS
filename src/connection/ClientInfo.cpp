/*
 * Information about one client.
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

#include "util/Serializer.hpp"
#include "util/Deserializer.hpp"
#include "ClientInfo.hpp"

Connection::ClientInfo::ClientInfo(const std::string& data) {
	Deserializer input(data);
	input.get(id);
	input.get(name);
	input.get(ai);

	int n;
	input.get(n);
	while (n--) {
		int id;
		input.get(id);
		players.insert(id);
	}

	input.get(readyToInit);
	input.get(readyToStart);
}

std::string Connection::ClientInfo::serialize() const {
	Serializer output;
	output.put(id);
	output.put(name);
	output.put(ai);

	output.put((int) players.size());
	for (Game::Player::IdType id: players) {
		output.put(id);
	}

	output.put(readyToInit);
	output.put(readyToStart);
	return output.getData();
}

/*
 * Game message class implementation.
 *
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

#include "Message.hpp"
#include "util/Serializer.hpp"
#include "util/Deserializer.hpp"

Game::Message::Message(const std::string& data) {
	Deserializer input(data);
	input.get(client);
	input.get(timestamp);
	input.get(action);
	input.get(position);
	Object::IdType id;
	while (input.get(id), id != 0) {
		actors.push_back(id);
	}
	while (input.get(id), id != 0) {
		targets.push_back(id);
	}
}

std::string Game::Message::serialize() const {
	Serializer output;
	output.put(client);
	output.put(timestamp);
	output.put(action);
	output.put(position);
	for (std::list<Object::IdType>::const_iterator i = actors.begin(); i != actors.end(); ++i) {
		output.put(*i);
	}
	output.put(Object::IdType(0));
	for (std::list<Object::IdType>::const_iterator i = targets.begin(); i != targets.end(); ++i) {
		output.put(*i);
	}
	output.put(Object::IdType(0));
	return output.getData();
}

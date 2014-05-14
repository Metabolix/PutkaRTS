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

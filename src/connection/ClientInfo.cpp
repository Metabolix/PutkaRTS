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

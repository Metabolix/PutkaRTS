/*
 * Communication with the metaserver.
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

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <boost/asio.hpp>

#include "ProgramInfo.hpp"
#include "Metaserver.hpp"
#include "Address.hpp"
#include "Server.hpp"

const std::string Connection::Metaserver::defaultUrl = "http://putkarts.dy.fi/metaserver/metaserver.php";

void Connection::Metaserver::luaLocation() {
	url = get<String>(1);
}

void Connection::Metaserver::luaGame() {
	Game game;
	game.id = get<Integer>(1);
	game.version = get<String>(2);
	game.name = get<String>(3);
	games[game.id] = game;
}

void Connection::Metaserver::luaGameAddress() {
	Integer id = get<Integer>(1);
	String address = get<String>(2);
	games[id].addresses.push_back(address);
}

Connection::Metaserver::Metaserver(const std::string& url_):
	url(url_) {
	bind("location", boost::bind(&Metaserver::luaLocation, this));
	bind("gameImpl", boost::bind(&Metaserver::luaGame, this));
	bind("gameAddressImpl", boost::bind(&Metaserver::luaGameAddress, this));
	run<void>(
		"function game(info)\n"
		"	gameImpl(info.id, info.version or 'DummyVersion', info.name or 'DummyName')\n"
		"	t = info.address or {}\n"
		"	for i = 1, # t do gameAddressImpl(info.id, t[i]) end\n"
		"end\n"
	);
}

bool Connection::Metaserver::sendGame(const Server& server) {
	// Refuse to send if the ttl has not expired.
	if (ttl.getTime() < 0) {
		return false;
	}
	ttl.reset(-5);

	// Build the POST data.
	std::string data = "game[version]=" + ProgramInfo::version;
	bool send = false;
	for (Server::ListenerContainerType::iterator i = server.listeners.begin(); i != server.listeners.end(); ++i) {
		try {
			std::string address = Address::toString(**i);
			data += "&address[]=" + address;
			send = true;
		} catch (...) {
		}
	}
	return send ? http(data) : false;
}

bool Connection::Metaserver::getGames() {
	// Refuse to fetch if the ttl has not expired.
	if (ttl.getTime() < 0) {
		return false;
	}
	ttl.reset(-5);

	games.clear();
	return http();
}

void Connection::Metaserver::extractUrlComponents(std::string& host, std::string& port, std::string& httpHost, std::string& httpPath) const {
	// Parse the url.
	std::string::size_type pos0, pos1, pos2;
	pos0 = url.find("//") + 2;
	pos2 = url.find('/', pos0);

	// pos1 = url.find(':', pos0) needs some tricks for IPv6 (http://[::]:80).
	pos1 = url.find_last_not_of("0123456789", pos2);
	if (url[pos1] != ':') {
		pos1 = std::string::npos;
	}

	host = url.substr(pos0, std::min(pos1, pos2) - pos0);
	port = (pos0 < pos1 && pos1 < pos2) ? url.substr(pos1 + 1, pos2 - pos1 - 1) : "80";
	httpHost = url.substr(pos0, pos2 - pos0);
	httpPath = (pos2 < url.size()) ? url.substr(pos2) : "/";
}

bool Connection::Metaserver::http(const std::string& postData) {
	std::string host, port, httpHost, httpPath;
	extractUrlComponents(host, port, httpHost, httpPath);

	// Connect.
	boost::asio::ip::tcp::iostream stream;
	stream.expires_from_now(boost::posix_time::seconds(3));
	stream.connect(host, port);

	// Send the request.
	stream << (postData.empty() ? "GET" : "POST") << " " << httpPath << " HTTP/1.1\r\n";
	stream << "Host: " << httpHost << "\r\n";
	stream << "X-PutkaRTS-Version: " << ProgramInfo::version << "\r\n";
	if (!secret.empty()) {
		stream << "Cookie: secret=" + secret + "\r\n";
	}
	if (!postData.empty()) {
		stream << "Content-Type: application/x-www-form-urlencoded\r\n";
		stream << "Content-Length: " << postData.size() << "\r\n";
	}
	stream << "Connection: close\r\n\r\n";
	stream << postData;

	// HTTP/1.0 200 OK\r\n
	std::string data;
	int status = 0;
	stream >> data >> status;
	std::getline(stream, data, '\n');

	if (!stream) {
		if (stream.expires_from_now() < boost::posix_time::seconds(0)) {
			throw std::runtime_error("Connection timed out.");
		}
		throw std::runtime_error(stream.error().message());
	}

	if (status >= 400 || status < 200) {
		boost::trim(data);
		throw std::runtime_error((boost::format("HTTP error: %d %s") % status % data).str());
	}

	// Reset the timer, there shouldn't be a timeout anymore.
	stream.expires_from_now(boost::posix_time::seconds(3));

	// Header-Name: value\r\n
	bool chunked = false;
	while (std::getline(stream, data, '\r')) {
		stream.ignore(1, '\n');
		if (data.empty()) {
			break;
		}
		std::string name = data.substr(0, data.find(':'));
		std::string value = data.substr(data.find(':') + 1);
		boost::to_lower(name);
		boost::trim(value);

		if (name == "transfer-encoding") {
			boost::to_lower(value);
			chunked = (value == "chunked");
		}
		if (name == "set-cookie") {
			std::string::size_type pos0, pos1;
			pos0 = value.find('=') + 1;
			pos1 = value.find(';', pos0);
			if (value.substr(0, pos0 - 1) == "secret") {
				secret = value.substr(pos0, pos1 - pos0);
			}
		}
		if (name == "x-putkarts-address") {
			localAddress = value;
		}
		if (name == "x-putkarts-ttl") {
			ttl.reset(-boost::lexical_cast<double>(value)); // Negative; it counts towards zero then.
		}
		if (name == "location") {
			url = value;
		}
	}

	// Get the response data.
	data.clear();
	if (!chunked) {
		// Use getline for reading, there won't be any NUL bytes.
		std::getline(stream, data, '\0');
	} else {
		// Chunked encoding: <hex-length>\r\n<data>\r\n until length = 0.
		size_t length = 0;
		do {
			stream >> std::hex >> length;
			stream.ignore(2); // \r\n
			while (length) {
				char buffer[1024];
				stream.read(buffer, std::min<size_t>(length, sizeof(buffer)));
				size_t n = stream.gcount();
				data.insert(data.end(), buffer, buffer + n);
				length -= n;
			}
		} while (length);
	}

	// Handle the response.
	if (status >= 200 && status < 300) {
		run<void>(data);
		return true;
	}
	return false;
}

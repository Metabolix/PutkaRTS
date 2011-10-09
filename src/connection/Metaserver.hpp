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

#ifndef PUTKARTS_Connection_Metaserver_HPP
#define PUTKARTS_Connection_Metaserver_HPP

#include <string>
#include <list>

#include "lua/Lua.hpp"
#include "util/Clock.hpp"

namespace Connection {
	class Metaserver;
	class Server;
}

/**
 * A class that communicates with the metaserver.
 */
class Connection::Metaserver: Lua {
public:
	/** The default url of the metaserver. */
	static const std::string defaultUrl;

	/** The url of the metaserver. */
	std::string url;

	/** Our own address. */
	std::string localAddress;

	/** The secret key to use for subsequent requests. */
	std::string secret;

	/** A clock for counting down the TTL. */
	Clock ttl;

	/**
	 * Constructor.
	 *
	 * @param url The url of the metaserver.
	 */
	Metaserver(const std::string& url = defaultUrl);

	/**
	 * Send a game to the metaserver.
	 *
	 * @param server The game server.
	 * @return True if the request was sent, false if it wasn't sent at all.
	 * @throw std::runtime_error Thrown if something goes wrong.
	 */
	bool sendGame(const Server& server);

private:
	/**
	 * Extract the relevant components of the url.
	 *
	 * @param host The host is stored here.
	 * @param port The port is stored here.
	 * @param httpHost The HTTP Host header is stored here.
	 * @param httpPath The path is stored here.
	 */
	void extractUrlComponents(std::string& host, std::string& port, std::string& httpHost, std::string& httpPath) const;

	/**
	 * Send a HTTP request to the metaserver and handle the response.
	 *
	 * @param postData The data to send using the POST method; if empty, use GET instead.
	 * @return True if the request was successfully sent, false if not.
	 * @throw std::runtime_error Thrown if something goes wrong.
	 */
	bool http(const std::string& postData = "");

	/**
	 * Lua callback: Set the metaserver url.
	 */
	void luaLocation();
};

#endif

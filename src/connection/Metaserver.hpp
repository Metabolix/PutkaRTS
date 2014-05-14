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

	/**
	 * A class that describes a game.
	 */
	struct Game {
		/** The id, as assigned by the metaserver. */
		int id;

		/** The version of the game server. */
		std::string version;

		/** The name of the game. */
		std::string name;

		/** The addresses for the game. */
		std::list<std::string> addresses;
	};

	/** The url of the metaserver. */
	std::string url;

	/** Our own address. */
	std::string localAddress;

	/** The secret key to use for subsequent requests. */
	std::string secret;

	/** A clock for counting down the TTL. */
	Clock ttl;

	/** The list of games received from the metaserver. */
	std::map<int, Game> games;

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

	/**
	 * Get the list of games.
	 *
	 * @return True if the list was updated, false if nothing changed.
	 * @throw std::runtime_error Thrown if something goes wrong.
	 */
	bool getGames();

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

	/**
	 * Lua callback: Set game info for a game.
	 */
	void luaGame();

	/**
	 * Lua callback: Add an address for a game.
	 */
	void luaGameAddress();
};

#endif

#ifndef PUTKARTS_Connection_ClientInfo_HPP
#define PUTKARTS_Connection_ClientInfo_HPP

#include <string>

#include "game/Client.hpp"

namespace Connection {
	class ClientInfo;
}

/**
 * Information about one client.
 */
class Connection::ClientInfo: public Game::Client {
public:
	/** Is this client ready to init the game? */
	bool readyToInit;

	/** Is this client ready to start playing? */
	bool readyToStart;

	/** Constructor; sets the default values. */
	ClientInfo():
		readyToInit(false),
		readyToStart(false) {
	}

	/**
	 * Virtual destructor.
	 */
	virtual ~ClientInfo() {}

	/**
	 * Create a client info from string.
	 */
	ClientInfo(const std::string& data);

	/**
	 * Conver a client info to string.
	 */
	std::string serialize() const;
};

#endif

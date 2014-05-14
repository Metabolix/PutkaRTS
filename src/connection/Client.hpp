#ifndef PUTKARTS_Connection_Client_HPP
#define PUTKARTS_Connection_Client_HPP

#include <string>

#include "util/Scalar.hpp"
#include "connection/Base.hpp"
#include "connection/EndPoint.hpp"

namespace Connection {
	class Client;
}

namespace Game {
	class Player;
}

/**
 * Base class for game clients.
 */
class Connection::Client: virtual public Connection::Base {
	/** Connection to the server. */
	std::shared_ptr<EndPoint> connection;

	/** The client id of this client. */
	int ownId;

	/** The greatest message timestamp. */
	Scalar<SIUnit::Time> lastMessageTimestamp;

	/** The greatest message timestamp before lastMessageTimestamp. */
	Scalar<SIUnit::Time> prevMessageTimestamp;

	/**
	 * Handle a received packet.
	 *
	 * @param data The packet.
	 */
	void handlePacket(std::string& data);

public:
	/**
	 * Construct a new client with the specified connection.
	 *
	 * @param conn Connection to a server.
	 */
	Client(std::shared_ptr<EndPoint> conn):
		connection(conn) {
	}

	/**
	 * Send a message describing a player action.
	 *
	 * @param message The message to send.
	 */
	virtual void sendMessage(const Game::Message& message);

	/**
	 * Handle data from the server, and update the game state.
	 */
	virtual void update();

	/**
	 * Get the client info for this client.
	 */
	std::shared_ptr<const ClientInfo> getClientInfo() const {
		return clients.find(ownId)->second;
	}

	/**
	 * Mark the client as ready for initialising the game ("settings ok, let's play").
	 */
	void setReadyToInit();

	/**
	 * Mark the client as ready for initialising the game ("all set, start the clock").
	 */
	void setReadyToStart();
};

#endif

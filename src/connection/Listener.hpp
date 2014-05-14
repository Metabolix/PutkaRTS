#ifndef PUTKARTS_Connection_Listener_HPP
#define PUTKARTS_Connection_Listener_HPP

#include <boost/utility.hpp>

namespace Connection {
	class Server;
	class Listener;
}

/**
 * Interface for listeners.
 */
class Connection::Listener: boost::noncopyable {
public:
	/**
	 * Virtual destructor.
	 */
	virtual ~Listener() {
		// Nothing to do.
	}

	/**
	 * Inform the server about changes.
	 *
	 * @param server The server.
	 * @return false if this Listener should be removed, true otherwise.
	 */
	virtual bool update(Server& server) = 0;
};

#endif

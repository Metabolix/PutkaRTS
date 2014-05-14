#ifndef PUTKARTS_Connection_Address_HPP
#define PUTKARTS_Connection_Address_HPP

#include <string>
#include <memory>

namespace Connection {
	class EndPoint;
	class Listener;
	class Address;
}

class Connection::Address {
	/** Make the class non-constructible. */
	virtual void makeNonConstructible() = 0;

public:
	/**
	 * Connect to an address.
	 *
	 * @param address The address.
	 * @return A pointer to the connection.
	 * @throw std::exception Thrown if something goes wrong.
	 */
	static std::shared_ptr<Connection::EndPoint> connect(const std::string& address);

	/**
	 * Get the address of a listener.
	 *
	 * @param listener The listener.
	 * @return The address of the listener.
	 * @throw std::exception Thrown if something goes wrong.
	 */
	static std::string toString(const Listener& listener);
};

#endif

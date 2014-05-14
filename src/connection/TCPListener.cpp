#include <boost/format.hpp>

#include "TCPListener.hpp"

Connection::TCPListener::TCPListener(const boost::asio::ip::tcp::endpoint& address):
	acceptor(service),
	next(new TCPEndPoint()) {
	acceptor.open(address.protocol());
	acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	if (address.address().is_v6()) {
		acceptor.set_option(boost::asio::ip::v6_only(true));
	}
	acceptor.non_blocking(true);
	acceptor.bind(address);
	acceptor.listen();
}

bool Connection::TCPListener::update(Server& server) {
	while (true) {
		boost::system::error_code error;
		acceptor.accept(next->socket, error);
		if (error) {
			return (error == boost::asio::error::would_block);
		}
		server.addClient(next);
		next.reset(new TCPEndPoint());
	}
	return true;
}

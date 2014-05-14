#include <iostream>
#include <stdexcept>

#include "ProgramInfo.hpp"
#include "util/Path.hpp"
#include "connection/Server.hpp"
#include "connection/TCPListener.hpp"

/**
 * Main function for the command-line interface.
 */
int main(int argc, char** argv)
try {
	Path::init(argc ? argv[0] : "./bin/unknown.exe");
	std::string title = ProgramInfo::name + " (version " + ProgramInfo::version + ", CLI)";
	std::cout << title << std::endl;
	std::shared_ptr<Connection::Server> server(new Connection::Server());

	int listeners = 0;
	try {
		std::cout << "Starting TCP listener on IPv6... ";
		server->addListener(std::make_shared<Connection::TCPListener>(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v6(), 6667)));
		std::cout << "OK!\n";
		++listeners;
	} catch (std::runtime_error& e) {
		std::cout << "Error! " << e.what() << "\n";
	}
	try {
		std::cout << "Starting TCP listener on IPv4... ";
		server->addListener(std::make_shared<Connection::TCPListener>(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 6667)));
		std::cout << "OK!\n";
		++listeners;
	} catch (std::runtime_error& e) {
		std::cout << "Error! " << e.what() << "\n";
	}
	if (!listeners) {
		std::cout << "No listeners, bailing out...\n";
		return 1;
	}
	std::cout << "Listeners added, starting the main loop." << std::endl;
	server->run();
	return 0;
} catch (std::exception& e) {
	std::cerr << "Fatal exception: " << e.what() << std::endl;
	return 1;
} catch (...) {
	std::cerr << "Fatal exception of unknown cause!" << std::endl;
	return 1;
}

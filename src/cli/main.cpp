/*
 * The CLI program entry point.
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

#include <iostream>
#include <stdexcept>
#include <boost/make_shared.hpp>

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
	boost::shared_ptr<Connection::Server> server(new Connection::Server());
	server->addListener(boost::make_shared<Connection::TCPListener>(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 6667)));
	while (true) {
		server->update();
	}
	return 0;
} catch (std::exception& e) {
	std::cerr << "Fatal exception: " << e.what() << std::endl;
	return 1;
} catch (...) {
	std::cerr << "Fatal exception of unknown cause!" << std::endl;
	return 1;
}

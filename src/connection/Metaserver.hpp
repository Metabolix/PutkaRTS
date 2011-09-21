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

namespace Connection {
	class Metaserver;
}

/**
 * A class that communicates with the metaserver.
 */
class Connection::Metaserver {
public:
	/** The default url of the metaserver. */
	static const std::string defaultUrl;

	/** The url of the metaserver. */
	std::string url;

	/**
	 * Constructor.
	 *
	 * @param url The url of the metaserver.
	 */
	Metaserver(const std::string& url = defaultUrl);
};

#endif

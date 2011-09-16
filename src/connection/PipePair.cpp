/*
 * End point of a connection.
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

#include <list>
#include <boost/thread.hpp>

#include "PipePair.hpp"

/**
 * An implementation of FIFO.
 */
class Connection::PipePair::Pipe: public Connection::EndPoint, private boost::mutex {
	/** Internal buffer. */
	std::list<std::string> buffer;

public:
	/** @copydoc EndPoint::sendPacket */
	void sendPacket(const std::string& data) {
		boost::lock_guard<boost::mutex> lock(*this);
		buffer.push_back(data);
	}

	/** @copydoc EndPoint::receivePacket */
	bool receivePacket(std::string& data) {
		if (buffer.empty()) {
			return false;
		}
		boost::lock_guard<boost::mutex> lock(*this);
		if (buffer.empty()) {
			return false;
		}
		data = *buffer.begin();
		buffer.pop_front();
		return true;
	}
};

/**
 * A class that takes two EndPoints and uses one for input and the other for output.
 */
class Connection::PipePair::Mixer: public Connection::EndPoint {
	boost::shared_ptr<EndPoint> input, output;

public:
	/**
	 * Construct an end point which reads from one stream and writes to another.
	 *
	 * @param i The input stream.
	 * @param o The output stream.
	 */
	Mixer(boost::shared_ptr<EndPoint> i, boost::shared_ptr<EndPoint> o):
		input(i),
		output(o) {
	}

	/** @copydoc EndPoint::sendPacket */
	void sendPacket(const std::string& data) {
		return output->sendPacket(data);
	}

	/** @copydoc EndPoint::receivePacket */
	bool receivePacket(std::string& data) {
		return input->receivePacket(data);
	}
};

Connection::PipePair::PipePair() {
	boost::shared_ptr<Pipe> p1(new Pipe());
	boost::shared_ptr<Pipe> p2(new Pipe());
	end1.reset(new Mixer(p1, p2));
	end2.reset(new Mixer(p2, p1));
}

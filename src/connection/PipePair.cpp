#include <list>
#include <mutex>

#include "PipePair.hpp"

/**
 * An implementation of FIFO.
 */
class Connection::PipePair::Pipe: public Connection::EndPoint, private std::mutex {
	/** Internal buffer. */
	std::list<std::string> buffer;

public:
	/** @copydoc EndPoint::sendPacket */
	void sendPacket(const std::string& data) {
		std::lock_guard<std::mutex> lock(*this);
		buffer.push_back(data);
	}

	/** @copydoc EndPoint::receivePacket */
	bool receivePacket(std::string& data) {
		if (buffer.empty()) {
			return false;
		}
		std::lock_guard<std::mutex> lock(*this);
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
	std::shared_ptr<EndPoint> input, output;

public:
	/**
	 * Construct an end point which reads from one stream and writes to another.
	 *
	 * @param i The input stream.
	 * @param o The output stream.
	 */
	Mixer(std::shared_ptr<EndPoint> i, std::shared_ptr<EndPoint> o):
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
	std::shared_ptr<Pipe> p1(new Pipe());
	std::shared_ptr<Pipe> p2(new Pipe());
	end1.reset(new Mixer(p1, p2));
	end2.reset(new Mixer(p2, p1));
}

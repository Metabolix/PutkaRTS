#include <stdexcept>
#include <vector>

#include "Deserializer.hpp"

static void finishValue(std::istream& data) {
	if (!data || data.get() != '\n') {
		throw std::runtime_error("Invalid data (missing '\n')!");
	}
}

void Deserializer::get(unsigned int& value) {
	data >> value;
	finishValue(data);
}

void Deserializer::get(int& value) {
	data >> value;
	finishValue(data);
}

void Deserializer::get(bool& value) {
	value = (data.get() == '1');
	finishValue(data);
}

void Deserializer::get(Scalar<>& value) {
	data >> value.value;
	finishValue(data);
}

void Deserializer::get(std::string& value) {
	std::vector<char> ret;
	unsigned int size;
	get(size);
	ret.resize(size);
	data.read(&ret[0], size);
	finishValue(data);
	value.assign(ret.begin(), ret.end());
}

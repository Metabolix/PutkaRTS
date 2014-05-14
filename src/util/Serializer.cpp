#include "Serializer.hpp"

void Serializer::put(const unsigned int& value) {
	data << value << '\n';
}

void Serializer::put(const int& value) {
	data << value << '\n';
}

void Serializer::put(const bool& value) {
	data << (value ? 1 : 0) << '\n';
}

void Serializer::put(const Scalar<>& value) {
	data << value.getDouble() << '\n';
}

void Serializer::put(const std::string& value) {
	unsigned int size = value.size();
	put(size);
	data.write(value.c_str(), size);
	data << '\n';
}

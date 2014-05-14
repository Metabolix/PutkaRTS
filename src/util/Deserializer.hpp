#ifndef PUTKARTS_Deserializer_HPP
#define PUTKARTS_Deserializer_HPP

#include <sstream>
#include <string>

#include "Scalar.hpp"
#include "Vector2.hpp"

/**
 * Class for deserializing data.
 */
class Deserializer {
	/** The data. */
	std::istringstream data;

public:
	/**
	 * Construct a new deserializer.
	 */
	Deserializer(const std::string& data_):
		data(data_) {
	}

	/**
	 * Deserialize an unsigned integer.
	 *
	 * @param value Reference to the object that shall hold the value.
	 */
	void get(unsigned int& value);

	/**
	 * Deserialize an integer.
	 *
	 * @param value Reference to the object that shall hold the value.
	 */
	void get(int& value);

	/**
	 * Deserialize a boolean.
	 *
	 * @param value Reference to the object that shall hold the value.
	 */
	void get(bool& value);

	/**
	 * Deserialize a Scalar<> object.
	 *
	 * @param value Reference to the object that shall hold the value.
	 */
	void get(Scalar<>& value);

	/**
	 * Deserialize a string.
	 *
	 * @param value Reference to the object that shall hold the value.
	 */
	void get(std::string& value);

	/**
	 * Deserialize any kind of Scalar<T> object.
	 *
	 * @param value Reference to the object that shall hold the value.
	 */
	template <typename T>
	inline void get(Scalar<T>& value) {
		Scalar<> tmp;
		get(tmp);
		value.unsafeAssign(tmp);
	}

	/**
	 * Deserialize any kind of Vector<T> object.
	 *
	 * @param value Reference to the object that shall hold the value.
	 */
	template <typename T>
	inline void get(Vector2<T>& value) {
		get(value.x);
		get(value.y);
	}
};

#endif

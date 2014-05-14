#ifndef PUTKARTS_Configuration_HPP
#define PUTKARTS_Configuration_HPP

#include <string>
#include <map>

/**
 * Class for configuration.
 *
 * This class can handle configuration files with following format:
 * [key] = [value]
 * [key] = [value]
 * ...
 */
class Configuration {
	/** Data structure for configuration. */
	std::map<std::string, std::string> configuration;

	/** Path of the configuration file. */
	std::string filepath;

	/** Remember whether the settings have changed since loading. */
	mutable bool changed;

	/**
	 * Returns the value of configuration key.
	 *
	 * @param key The configuration key.
	 * @param defaultValue Default value.
	 * @return Value of configuration key. If key is not found, return value is defaultValue.
	 * @throw std::logic_error Thrown if key is illegal (empty, starts with '#' or has whitespace around it).
	 */
	template <typename T>
	T getValue(const std::string& key, T defaultValue) const;

	/**
	 * Sets the value of configuration key.
	 *
	 * @param key The configuration key.
	 * @param value The value to set.
	 * @throw std::logic_error Thrown if key is illegal (empty, starts with '#' or has whitespace around it).
	 */
	template <typename T>
	void setValue(const std::string& key, T value);

public:

	/**
	 * Constructor.
	 */
	Configuration();

	/**
	 * Constructor. Loads configuration from file.
	 *
	 * @param filepath Path of the configuration file.
	 */
	Configuration(const std::string& filepath);

	/**
	 * Loads the configuration from stream.
	 *
	 * @param stream Stream to read configuration.
	 */
	void load(std::istream& stream);

	/**
	 * Loads the configuration from file.
	 *
	 * @param filepath Path of the configuration file.
	 * @return Whether the loading succeeded.
	 */
	bool load(const std::string& filepath);

	/**
	 * Loads the configuration from the file it was last loaded from.
	 *
	 * @return Whether the loading succeeded.
	 */
	bool load();

	/**
	 * Saves the configuration to a stream.
	 *
	 * @param stream The stream to save the configuration.
	 * @throw std::runtime_error Thrown if failed to open the file.
	 */
	void save(std::ostream& stream) const;

	/**
	 * Saves the configuration to a spesicif file.
	 *
	 * @param filepath Name of the file to save the configuration.
	 * @throw std::runtime_error Thrown if opening file fails.
	 */
	void save(const std::string& filepath) const;

	/**
	 * Saves the configuration to the file it was loaded from.
	 *
	 * @throw std::runtime_error Thrown if failed to open the file.
	 */
	void save() const;

	/**
	 * Returns the bool value of configuration key.
	 *
	 * @param key The configuration key.
	 * @param defaultValue Default value.
	 * @return Value of configuration key. If key is not found, return value is defaultValue.
	 * @throw std::logic_error Thrown if key is illegal (empty, starts with '#' or has whitespace around it).
	 */
	bool getBool(const std::string& key, bool defaultValue) const;

	/**
	 * Returns the int value of configuration key.
	 *
	 * @param key The configuration key.
	 * @param defaultValue Default value.
	 * @return Value of configuration key. If key is not found, return value is defaultValue.
	 * @throw std::logic_error Thrown if key is illegal (empty, starts with '#' or has whitespace around it).
	 */
	int getInt(const std::string& key, int defaultValue) const;

	/**
	 * Returns the double value of configuration key.
	 *
	 * @param key The configuration key.
	 * @param defaultValue Default value.
	 * @return Value of configuration key. If key is not found, return value is defaultValue.
	 * @throw std::logic_error Thrown if key is illegal (empty, starts with '#' or has whitespace around it).
	 */
	double getDouble(const std::string& key, double defaultValue) const;

	/**
	 * Returns the string value of configuration key.
	 *
	 * @param key The configuration key.
	 * @param defaultValue Default value.
	 * @return Value of configuration key. If key is not found, return value is defaultValue.
	 * @throw std::logic_error Thrown if key is illegal (empty, starts with '#' or has whitespace around it).
	 */
	std::string getString(const std::string& key, const std::string& defaultValue) const;

	/**
	 * Sets the bool value of configuration key.
	 *
	 * @param key The configuration key.
	 * @param value The value to set.
	 * @throw std::logic_error Thrown if key is illegal (empty, starts with '#' or has whitespace around it).
	 */
	void setBool(const std::string& key, bool value);

	/**
	 * Sets the int value of configuration key.
	 *
	 * @param key The configuration key.
	 * @param value The value to set.
	 * @throw std::logic_error Thrown if key is illegal (empty, starts with '#' or has whitespace around it).
	 */
	void setInt(const std::string& key, int value);

	/**
	 * Sets the double value of configuration key.
	 *
	 * @param key The configuration key.
	 * @param value The value to set.
	 * @throw std::logic_error Thrown if key is illegal (empty, starts with '#' or has whitespace around it).
	 */
	void setDouble(const std::string& key, double value);

	/**
	 * Sets the string value of configuration key.
	 *
	 * @param key The configuration key.
	 * @param value The value to set.
	 * @throw std::logic_error Thrown if key is illegal (empty, starts with '#' or has whitespace around it).
	 */
	void setString(const std::string& key, const std::string& value);

	/**
	 * Removes the key from configuration.
	 *
	 * @param key Key to remove.
	 */
	void remove(const std::string& key);

	/**
	 * Checks wether the key is set.
	 *
	 * @param key Key to check.
	 */
	bool has(const std::string& key) const;

	/**
	 * Clears configuration.
	 */
	void clear();
};

#endif

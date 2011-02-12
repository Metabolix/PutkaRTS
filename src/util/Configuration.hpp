/*
 * Configuration class definition.
 *
 * Copyright 2011 Petri Österman
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PutkaRTS. If not, see <http://www.gnu.org/licenses/>.
 */

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
	/** Datastructure for users and configuration */
	std::map<std::string, std::string> configuration;

	/** Path of the configuration file */
	std::string filepath;

	/**
	 * Returns the value of configuration key.
	 *
	 * @param key The configuration key
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
	Configuration(const std::string& filepath_);

	/**
	 * Loads the configuration from file.
	 *
	 * @param filepath Path of the configuration file.
	 * @return Whether the loading succeeded.
	 */
	bool load(const std::string& filepath_);

	/**
	 * Saves the configuration to the file they were loaded from.
	 *
	 * @throw std::runtime_error Thrown if failed to open the file.
	 */
	void save() const;

	/**
	 * Saves the configuration to a spesicif file.
	 *
	 * If method fails to open file, it throws runtime_error.
	 *
	 * @param filename Name of the file to save the configuration.
	 * @throw std::runtime_error Thrown if opening file fails.
	 */
	void save(const std::string& filepath_) const;

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
	 * @param key key to remove.
	 */
	void remove(const std::string& key);
};

#endif

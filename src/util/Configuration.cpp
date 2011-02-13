/*
 * Configuration class implementation.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PutkaRTS.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/algorithm/string.hpp>

#include "Configuration.hpp"

Configuration::Configuration() {
}

Configuration::Configuration(const std::string& filepath_):
	filepath(filepath_) {
	// Load default settings.
	load(filepath);
}

void Configuration::load(std::istream& stream) {
	// Clear possible earlier configuration.
	configuration.clear();

	// Read all keys and values.
	std::string line;
	while (getline(stream, line)) {
		// Skip all lines starting with '#'
		if (line.empty() || line[0] == '#') {
			continue;
		}

		// Find separator '='.
		std::string::size_type position = line.find('=');
		if (position == std::string::npos) {
			std::cerr << "Illegal configuration line: \"" << line << "\"" << std::endl;
			continue;
		}

		// Read paremeter and value.
		std::string key = boost::trim_copy(line.substr(0, position));
		std::string value = boost::trim_copy(line.substr(position + 1));

		if (!key.empty() && key[0] != '#') {
			// Add them to data structure.
			configuration.insert(std::make_pair(key, value));
		} else {
			std::cerr << "Illegal configuration line: \"" << line << "\"" << std::endl;
		}
	}
}

bool Configuration::load(const std::string& filepath_) {
	// Open the configuration file.
	std::ifstream file(filepath_.c_str());
	if (!file) {
		std::cerr << "Failed to open configuration file: \"" << filepath << "\"" << std::endl;
		return false;
	}

	// Save filepath.
	filepath = filepath_;

	// Parse data.
	load(file);

	return true;
}

bool Configuration::load() {
	return load(filepath);
}

void Configuration::save(std::ostream& stream) const {
	// Write configuration to the stream.
	for (std::map<std::string, std::string>::const_iterator iter = configuration.begin(); iter != configuration.end(); ++iter) {
		stream << iter->first << " = " << iter->second << std::endl;
	}
}

void Configuration::save(const std::string& filepath_) const {
	// Open the configuration file.
	std::ofstream file(filepath_.c_str());
	if (!file) {
		throw std::runtime_error("Configuration::save: Failed to open configuration file: " + filepath_);
	}

	// Write configuration to the file.
	save(file);
}

void Configuration::save() const {
	save(filepath);
}

bool Configuration::getBool(const std::string& key, bool defaultValue) const {
	return getValue(key, defaultValue);
}

int Configuration::getInt(const std::string& key, int defaultValue) const {
	return getValue(key, defaultValue);
}

double Configuration::getDouble(const std::string& key, double defaultValue) const {
	return getValue(key, defaultValue);
}

std::string Configuration::getString(const std::string& key, const std::string& defaultValue) const {
	// If key is empty, starts with '#' or is not trimmed.
	if (key.empty() || key[0] == '#' || boost::trim_copy(key) != key) {
		throw std::logic_error("Configuration::getValue: Illegal key: \"" + key + "\"");
	}

	// If the key exists.
	if (configuration.find(key) != configuration.end()) {
		// Return it's value.
		return configuration.find(key)->second;
	}

	return defaultValue;
}

void Configuration::setBool(const std::string& key, bool value) {
	setValue(key, value);
}

void Configuration::setInt(const std::string& key, int value) {
	setValue(key, value);
}

void Configuration::setDouble(const std::string& key, double value) {
	setValue(key, value);
}

void Configuration::setString(const std::string& key, const std::string& value) {
	setValue(key, value);
}

void Configuration::remove(const std::string& key) {
	configuration.erase(key);
}

void Configuration::clear() {
	configuration.clear();
}

template <typename T>
T Configuration::getValue(const std::string& key, T defaultValue) const {
	// Get key as string. This takes care of all legality checks.
	std::istringstream stream(getString(key, ""));

	// Convert to type T.
	T value;
	if (stream >> value) {
		return value;
	}

	return defaultValue;
}

template <typename T>
void Configuration::setValue(const std::string& key, T value) {
	// If key is empty, starts with '#' or is not trimmed.
	if (key.empty() || key[0] == '#' || boost::trim_copy(key) != key) {
		throw std::logic_error("Configuration::setValue: Illegal key: \"" + key + "\"");
	}

	// Convert to string and save.
	std::ostringstream stream;
	stream << value;
	configuration[key] = stream.str();
}

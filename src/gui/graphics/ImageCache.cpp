/*
 * Image caching system.
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

#include <stdexcept>
#include <string>
#include <map>

#include "ImageCache.hpp"

#include <SFML/Graphics.hpp>

/**
 * Structure to hold an image and its reference count.
 */
struct GUI::ImageCache::Node {
	std::string file;
	sf::Image image;
	Node(std::string file_): file(file_) {
		if (!image.LoadFromFile(file)) {
			throw std::runtime_error(file + " could not be loaded!");
		}
	}
};

/** All the images that are currently loaded, listed by file name. */
std::map<std::string, boost::weak_ptr<GUI::ImageCache::Node> > GUI::ImageCache::known;

const sf::Image& GUI::ImageCache::get(const std::string& id, const std::string& file) {
	// Check if the image is already loaded.
	if (loaded.find(id) != loaded.end()) {
		const Node& node = *loaded.find(id)->second;
		if (node.file != file) {
			throw std::runtime_error("ImageCache already contains '" + id + "' (loaded from " + node.file + "), can't load " + file + "!");
		}
		return node.image;
	}

	boost::shared_ptr<Node> node;

	// Check if some other cache already has the image.
	if (known.find(file) != known.end()) {
		node = known.find(file)->second.lock();
	}

	// Load the image and store a weak reference.
	if (!node) {
		node.reset(new Node(file));
		known[file] = node;
	}
	loaded[id] = node;

	return node->image;
}

const sf::Image& GUI::ImageCache::get(const std::string& fileOrId) {
	if (loaded.find(fileOrId) != loaded.end()) {
		return loaded.find(fileOrId)->second->image;
	}
	return get(fileOrId, fileOrId);
}

const sf::Image& GUI::ImageCache::get(const std::string& id) const {
	if (loaded.find(id) != loaded.end()) {
		return loaded.find(id)->second->image;
	}
	throw std::runtime_error("ImageCache does not contain '" + id + "'!");
}

void GUI::ImageCache::clear() {
	loaded.clear();
}

GUI::ImageCache& GUI::ImageCache::operator = (const ImageCache& other) {
	if (this != &other) {
		loaded = other.loaded;
	}
	return *this;
}

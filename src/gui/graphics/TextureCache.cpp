/*
 * Texture caching system.
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

#include "TextureCache.hpp"

#include <SFML/Graphics.hpp>

/**
 * Structure to hold an Texture and its reference count.
 */
struct GUI::TextureCache::Node {
	std::string file;
	sf::Texture texture;
	Node(std::string file_): file(file_) {
		if (!texture.loadFromFile(file)) {
			throw std::runtime_error(file + " could not be loaded!");
		}
	}
};

/** All the textures that are currently loaded, listed by file name. */
std::map<std::string, std::weak_ptr<GUI::TextureCache::Node> > GUI::TextureCache::known;

const sf::Texture& GUI::TextureCache::get(const std::string& id, const std::string& file) {
	// Check if the Texture is already loaded.
	if (loaded.find(id) != loaded.end()) {
		const Node& node = *loaded.find(id)->second;
		if (node.file != file) {
			throw std::runtime_error("TextureCache already contains '" + id + "' (loaded from " + node.file + "), can't load " + file + "!");
		}
		return node.texture;
	}

	std::shared_ptr<Node> node;

	// Check if some other cache already has the texture.
	if (known.find(file) != known.end()) {
		node = known.find(file)->second.lock();
	}

	// Load the texture and store a weak reference.
	if (!node) {
		node.reset(new Node(file));
		known[file] = node;
	}
	loaded[id] = node;

	return node->texture;
}

const sf::Texture& GUI::TextureCache::get(const std::string& fileOrId) {
	if (loaded.find(fileOrId) != loaded.end()) {
		return loaded.find(fileOrId)->second->texture;
	}
	return get(fileOrId, fileOrId);
}

const sf::Texture& GUI::TextureCache::get(const std::string& id) const {
	if (loaded.find(id) != loaded.end()) {
		return loaded.find(id)->second->texture;
	}
	throw std::runtime_error("TextureCache does not contain '" + id + "'!");
}

void GUI::TextureCache::clear() {
	loaded.clear();
}

GUI::TextureCache& GUI::TextureCache::operator = (const TextureCache& other) {
	if (this != &other) {
		loaded = other.loaded;
	}
	return *this;
}

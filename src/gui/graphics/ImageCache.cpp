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

#include "ImageCache.hpp"

#include <SFML/Graphics.hpp>

#include <stdexcept>
#include <string>
#include <map>

/**
 * Structure to hold an image and its reference count.
 */
struct ImageCacheNode {
	sf::Image image;
	int references;
};

/** All the images that are currently loaded, listed by file name. */
static std::map<std::string, ImageCacheNode> images;

const sf::Image& GUI::ImageCache::get(const std::string& id, const std::string& file) {
	if (loaded.find(id) != loaded.end()) {
		if (loaded.find(id)->second != file) {
			throw std::runtime_error("ImageCache already contains '" + id + "' (loaded from " + loaded.find(id)->second + "), can't load " + file + "!");
		}
		return images[file].image;
	}

	if (images.find(file) == images.end()) {
		images[file].references = 0;
		if (!images[file].image.LoadFromFile(file)) {
			images.erase(file);
			throw std::runtime_error(file + " could not be loaded!");
		}
	}

	loaded[id] = file;
	images[file].references += 1;

	return images[file].image;
}

const sf::Image& GUI::ImageCache::get(const std::string& fileOrId) {
	if (loaded.find(fileOrId) != loaded.end()) {
		const ImageCache& tmp = *this;
		return tmp.get(fileOrId);
	}
	return get(fileOrId, fileOrId);
}

const sf::Image& GUI::ImageCache::get(const std::string& id) const {
	if (loaded.find(id) == loaded.end()) {
		throw std::runtime_error("ImageCache does not contain '" + id + "'!");
	}
	return images[loaded.find(id)->second].image;
}

void GUI::ImageCache::clear() {
	for (FileMap::iterator i = loaded.begin(); i != loaded.end(); ++i) {
		images[i->second].references -= 1;
		if (!images[i->second].references) {
			images.erase(i->second);
		}
	}
	loaded.clear();
}

GUI::ImageCache& GUI::ImageCache::operator = (const ImageCache& other) {
	if (this == &other) {
		return *this;
	}
	clear();
	loaded = other.loaded;
	for (FileMap::iterator i = loaded.begin(); i != loaded.end(); ++i) {
		images[i->second].references += 1;
	}
	return *this;
}

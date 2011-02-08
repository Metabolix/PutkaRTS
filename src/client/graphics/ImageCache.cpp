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

struct ImageCacheNode {
	sf::Image image;
	int references;
};

static std::map<std::string, ImageCacheNode> images;

const sf::Image& ImageCache::get(const std::string& file) {
	if (images.find(file) == images.end()) {
		images[file].references = 0;
		if (!images[file].image.LoadFromFile(file)) {
			images.erase(file);
			throw std::runtime_error(file + " could not be loaded!");
		}
	}

	if (!loaded[file]) {
		loaded[file] = true;
		++images[file].references;
	}

	return images[file].image;
}

ImageCache::~ImageCache() {
	for (std::map<std::string, bool>::iterator i = loaded.begin(); i != loaded.end(); ++i) {
		images[i->first].references--;
		if (!images[i->first].references) {
			images.erase(i->first);
		}
	}
}

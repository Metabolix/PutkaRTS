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

#ifndef _IMAGECACHE_HPP
#define _IMAGECACHE_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <map>

/**
 * Class for loading and automatically caching images.
 */
class ImageCache {
	/** List of currently loaded images; the bool is not used. */
	std::map<std::string, bool> loaded;
public:
	/**
	 * Load an image or fetch it from cache.
	 *
	 * @param file Image file name.
	 * @return reference to the image.
	 * @throw std::runtime_error Thrown if the image can't be loaded.
	 */
	const sf::Image& get(const std::string& file);

	/**
	 * Destructor; free images in this cache.
	 */
	~ImageCache();
};

#endif

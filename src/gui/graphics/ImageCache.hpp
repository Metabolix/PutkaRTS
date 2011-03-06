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

#ifndef PUTKARTS_GUI_ImageCache_HPP
#define PUTKARTS_GUI_ImageCache_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <map>

namespace GUI {
	class ImageCache;
}

/**
 * Class for loading and automatically caching images.
 */
class GUI::ImageCache {
	/** Type of the map used to keep track of loaded files. */
	typedef std::map<std::string, std::string> FileMap;

	/** List of currently loaded images and their file names. */
	FileMap loaded;
public:
	/**
	 * Fetch an image from the cache.
	 *
	 * @param id Image identifier.
	 * @return reference to the image.
	 * @throw std::runtime_error Thrown if the image is not loaded.
	 */
	const sf::Image& get(const std::string& id) const;

	/**
	 * Load an image or fetch it from cache.
	 *
	 * If there is an image with the given id, it will be returned.
	 * Otherwise the parameter will be used as a filename.
	 *
	 * @param fileOrId Image identifier or file name.
	 * @return reference to the image.
	 * @throw std::runtime_error Thrown if the image can't be loaded or if another image is already assigned to this name.
	 */
	const sf::Image& get(const std::string& fileOrId);

	/**
	 * Load an image or fetch it from cache.
	 *
	 * @param id An identifier for the image.
	 * @param file Image file name.
	 * @return reference to the image.
	 * @throw std::runtime_error Thrown if the image can't be loaded or if another image is already assigned to this name.
	 */
	const sf::Image& get(const std::string& id, const std::string& file);

	/**
	 * Free images in this cache.
	 */
	void clear();

	/**
	 * Assignment operator; update image reference counts.
	 *
	 * @param other The other cache.
	 * @return Reference to self.
	 */
	ImageCache& operator = (const ImageCache& other);

	/**
	 * Default constructor.
	 */
	ImageCache() {
	}

	/**
	 * Copy constructor; reuse assignment operator here.
	 *
	 * @param other The other cache.
	 */
	ImageCache(const ImageCache& other) {
		*this = other;
	}

	/**
	 * Destructor; free images in this cache.
	 */
	~ImageCache() {
		clear();
	}
};

#endif

#ifndef PUTKARTS_GUI_TextureCache_HPP
#define PUTKARTS_GUI_TextureCache_HPP

#include <string>
#include <map>
#include <memory>

#include <SFML/Graphics.hpp>

namespace GUI {
	class TextureCache;
}

/**
 * Class for loading and automatically caching textures.
 */
class GUI::TextureCache {
	/** Structure to hold a texture and its reference count. */
	class Node;

	/** List of currently loaded textures and their file names. */
	static std::map<std::string, std::weak_ptr<Node> > known;

	/** List of currently loaded textures and their file names. */
	std::map<std::string, std::shared_ptr<Node> > loaded;
public:
	/**
	 * Fetch a texture from the cache.
	 *
	 * @param id texture identifier.
	 * @return reference to the texture.
	 * @throw std::runtime_error Thrown if the texture is not loaded.
	 */
	const sf::Texture& get(const std::string& id) const;

	/**
	 * Load a texture or fetch it from cache.
	 *
	 * If there is a texture with the given id, it will be returned.
	 * Otherwise the parameter will be used as a filename.
	 *
	 * @param fileOrId texture identifier or file name.
	 * @return reference to the texture.
	 * @throw std::runtime_error Thrown if the texture can't be loaded or if another texture is already assigned to this name.
	 */
	const sf::Texture& get(const std::string& fileOrId);

	/**
	 * Load a texture or fetch it from cache.
	 *
	 * @param id An identifier for the texture.
	 * @param file texture file name.
	 * @return reference to the texture.
	 * @throw std::runtime_error Thrown if the texture can't be loaded or if another texture is already assigned to this name.
	 */
	const sf::Texture& get(const std::string& id, const std::string& file);

	/**
	 * Free textures in this cache.
	 */
	void clear();

	/**
	 * Assignment operator; update texture reference counts.
	 *
	 * @param other The other cache.
	 * @return Reference to self.
	 */
	TextureCache& operator = (const TextureCache& other);

	/**
	 * Default constructor.
	 */
	TextureCache() {
	}

	/**
	 * Copy constructor; reuse assignment operator here.
	 *
	 * @param other The other cache.
	 */
	TextureCache(const TextureCache& other) {
		*this = other;
	}

	/**
	 * Destructor; free textures in this cache.
	 */
	~TextureCache() {
		clear();
	}
};

#endif

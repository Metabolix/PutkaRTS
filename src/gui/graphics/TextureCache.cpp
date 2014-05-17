#include <stdexcept>
#include <string>
#include <map>

#include "TextureCache.hpp"

#include <SFML/Graphics.hpp>

/**
 * Structure to hold an Texture and its reference count.
 */
class GUI::TextureCache::Node {
	friend TextureCache;
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

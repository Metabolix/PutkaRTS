#ifndef PUTKARTS_GUI_Game_Map_HPP
#define PUTKARTS_GUI_Game_Map_HPP

#include <unordered_map>
#include <vector>

#include "game/Map.hpp"

#include <SFML/Graphics.hpp>

namespace GUI {
	namespace Game {
		class Map;
	}
}

/**
 * GUI class that wraps a game map and handles drawing it.
 */
class GUI::Game::Map: public sf::Drawable {
	/** Vertex arrays which contain blocks of the map. */
	std::vector<std::vector<sf::VertexArray> > blocks;

	/** The width of a vertex array in tiles. */
	const int blockWidthInTiles = 64;

	/** How many blocks the map contains. */
	sf::Vector2i blockCount;

	/** Index of tile names. */
	std::unordered_map<std::string, sf::Vector2u> tilePositions;

	/** Tileset that contains map tiles. */
	const sf::Texture tileset;

	/** Used to scale the map right size. */
	sf::Transformable transform;

public:
	/**
	 * Constructor.
	 *
	 * @param map The map used in game.
	 * @param tileSize Size of one tile.
	 */
	Map(const ::Game::Map& map, const sf::Vector2u& tileSize);

private:
	/**
	 * Draw the map part in target view.
	 *
	 * @param target Where we are going to draw.
	 * @param states How we are going to draw that.
	 */
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	/**
	 * Populate the vertex array "blocks", with one quad per tile in the map.
	 *
	 * @param map The map used in game.
	 * @param tileSize Size of one tile.
	 */
	void populateVertexArrays(const ::Game::Map& map, const sf::Vector2u& tileSize);

	/**
	 * Make map texture pack from tile images.
	 *
	 * @param map The map used in game.
	 * @param tileSize Size of one tile.
	 * @return map texture pack.
	 * @throw std::runtime_error Thrown if image tile doesn't exist.
	 * @throw std::runtime_error Thrown if image size doesn't match tile size.
	 */
	sf::Texture makeMapTexture(const ::Game::Map& map, const sf::Vector2u& tileSize);
};

#endif

#include <unordered_set>

#include "Map.hpp"

GUI::Game::Map::Map(const ::Game::Map& map, const sf::Vector2u& tileSize):
	tileset(makeMapTexture(map, tileSize)) {
	populateVertexArrays(map, tileSize);

	transform.scale(1.f / tileSize.x, 1.f / tileSize.y);
}

void GUI::Game::Map::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	// Check what part of the map we can see.
	sf::Vector2f viewPos = target.getView().getCenter() - target.getView().getSize() * 0.5f;
	int left = static_cast<int>(viewPos.x / blockWidthInTiles);
	int top = static_cast<int>(viewPos.y / blockWidthInTiles);
	viewPos += target.getView().getSize();
	int right = 1 + static_cast<int>(viewPos.x / blockWidthInTiles);
	int bottom = 1 + static_cast<int>(viewPos.y / blockWidthInTiles);

	// Clamp these to fit into array bounds.
	left = std::max(0, std::min(left, blockCount.x));
	top = std::max(0, std::min(top, blockCount.y));
	right = std::max(0, std::min(right, blockCount.x));
	bottom = std::max(0, std::min(bottom, blockCount.y));

	states.transform = transform.getTransform();
	states.texture = &tileset;

	for (int x = left; x < right; ++x) {
		for (int y = top; y < bottom; ++y) {
			target.draw(blocks[x][y], states);
		}
	}
}

void GUI::Game::Map::populateVertexArrays(const ::Game::Map& map, const sf::Vector2u& tileSize) {
	blockCount.x = (map.getSizeX() / blockWidthInTiles) + 1;
	blockCount.y = (map.getSizeY() / blockWidthInTiles) + 1;
	blocks.assign(blockCount.x, std::vector<sf::VertexArray>(blockCount.y, sf::VertexArray(sf::Quads)));

	for (unsigned int x = 0; x < map.getSizeX(); ++x) {
		for (unsigned int y = 0; y < map.getSizeY(); ++y) {
			const sf::Vector2u tilePosition = tilePositions[map(x, y).texture];

			const int blockX = x / blockWidthInTiles;
			const int blockY = y / blockWidthInTiles;

			sf::Vertex vertex;
			vertex.position = sf::Vector2f(x * tileSize.x, y * tileSize.y);
			vertex.texCoords = sf::Vector2f(tilePosition.x * tileSize.x, tilePosition.y * tileSize.y);
			blocks[blockX][blockY].append(vertex);

			vertex.position = sf::Vector2f((x + 1) * tileSize.x, y * tileSize.y);
			vertex.texCoords = sf::Vector2f((tilePosition.x + 1) * tileSize.x, tilePosition.y * tileSize.y);
			blocks[blockX][blockY].append(vertex);

			vertex.position = sf::Vector2f((x + 1) * tileSize.x, (y + 1) * tileSize.y);
			vertex.texCoords = sf::Vector2f((tilePosition.x + 1) * tileSize.x, (tilePosition.y + 1) * tileSize.y);
			blocks[blockX][blockY].append(vertex);

			vertex.position = sf::Vector2f(x * tileSize.x, (y + 1) * tileSize.y);
			vertex.texCoords = sf::Vector2f(tilePosition.x * tileSize.x, (tilePosition.y + 1) * tileSize.y);
			blocks[blockX][blockY].append(vertex);
		}
	}
}

sf::Texture GUI::Game::Map::makeMapTexture(const ::Game::Map& map, const sf::Vector2u& tileSize) {
	// Add texture names to a set to avoid duplicates.
	std::unordered_set<std::string> textureSet;
	for (const auto& i: map.getTileInfoMap()) {
		textureSet.emplace(i.second.texture);
	}

	sf::Texture texture;
	const int tilesInAxle = ceil(sqrt(textureSet.size()));
	texture.create(tilesInAxle * tileSize.x, tilesInAxle * tileSize.y);

	for (const auto& i: textureSet) {
		sf::Image image;
		if (!image.loadFromFile(Path::findDataPath(map.getDirectory(), "", i))) {
			throw std::runtime_error(i + " could not be loaded!");
		} else if (image.getSize().x != tileSize.x || image.getSize().y != tileSize.y) {
			throw std::runtime_error(i + " is not of the right size!");
		}
		// Calculate the position of the tile in the texture.
		const int tileX = tilePositions.size() % tilesInAxle;
		const int tileY = tilePositions.size() / tilesInAxle;
		texture.update(image, tileX * tileSize.x, tileY * tileSize.y);

		tilePositions.emplace(i, sf::Vector2u(tileX, tileY));
	}
	return texture;
}

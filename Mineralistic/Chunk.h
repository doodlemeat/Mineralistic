#pragma once

#include <vector>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>

class Tile;
class World;
class b2World;

namespace noise
{
	namespace utils
	{
		class NoiseMap;
	}
}

class Chunk: public sf::Drawable
{
public:
	Chunk(World *pWorld, b2World *pB2World);
	~Chunk();

	void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	bool isPosition(sf::Vector2i pPosition);
	void setPosition(sf::Vector2i pPosition);
	void setTexture(sf::Texture *pTexture);
	void buildChunk(noise::utils::NoiseMap *pHeightMap);
	sf::Vector2i getPosition();

	// Global chunk position
	Tile *getTileAt(sf::Vector2f pPosition);

	// Local chunk position
	Tile *getTile(sf::Vector2i pPosition);
	World *getWorld();
	b2World *getB2World();
private:
	World* mWorld;
	b2World *mB2World;
	sf::Vector2i mPosition;
	std::vector<std::vector<Tile*>> mTiles;

	sf::Texture mTexture;
	sf::VertexArray mVertices;
};


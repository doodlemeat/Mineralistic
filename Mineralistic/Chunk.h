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
	bool isDrawLight();
	bool isHighlight();

	void setPosition(sf::Vector2i pPosition);
	void setTexture(sf::Texture *pTexture);
	void buildChunk(noise::utils::NoiseMap *pHeightMap);
	void setDrawLight(bool pValue);
	void setHighlight(bool pValue);

	sf::Vector2i getPosition();

	// Global chunk position
	Tile *getTileAt(sf::Vector2f pPosition);

	Tile *getTile(sf::Vector2i pPosition);
	Tile *getTile(int x, int y);

	World *getWorld();
	b2World *getB2World();
	sf::Vertex *getVertices(sf::Vector2f pWorldPosition);
	Chunk *getRelative(sf::Vector2i pRelativePosition);
	bool equalAtWorldPosition(sf::Vector2f pWorldPosition);

private:
	bool mDrawLight;
	World* mWorld;
	b2World *mB2World;
	sf::Vector2i mPosition;
	Tile***mTiles;

	sf::Texture mTexture;
	sf::VertexArray mVertices;
	sf::VertexArray mLightVertices;
	bool mHighlight;
};


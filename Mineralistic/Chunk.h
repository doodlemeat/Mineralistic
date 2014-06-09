#pragma once

#include <vector>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>

class Tile;
class World;

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
	Chunk(World *pWorld);
	~Chunk();

	void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	sf::Sprite *getSprite();

	bool isPosition(sf::Vector2i pPosition);
	void setPosition(sf::Vector2i pPosition);
	void setTexture(sf::Texture *pTexture);
	void buildChunk(noise::utils::NoiseMap *pHeightMap);
	sf::Vector2i getPosition();
private:
	World* mWorld;
	sf::Vector2i mPosition;
	std::vector<std::vector<Tile*>> mTiles;

	//
	sf::Sprite *sprite;
	//

	sf::Texture mTexture;
	sf::VertexArray mVertices;
};


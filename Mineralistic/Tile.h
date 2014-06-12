#pragma once

#include <SFML/System/Vector2.hpp>

class Material;
class Chunk;
class b2Body;

namespace sf
{
	class Vertex;
}

class Tile
{
public:
	Tile(Chunk *pChunk, sf::Vector2i pPosition, Material *pMaterial, b2Body *pBody, sf::Vertex *pQuad);
	~Tile();
	sf::Vector2i getPosition();
	Material *getMaterial();
	b2Body *getBody();
	void setBodyNull();
	void setMaterial(Material *pMaterial);

private:
	sf::Vector2i mPosition;
	Chunk *mChunk;
	Material *mMaterial;
	b2Body *mBody;
	sf::Vertex *mQuad;
};


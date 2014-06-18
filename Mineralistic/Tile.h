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
	Tile(Chunk *pChunk, sf::Vector2f pPosition, Material *pMaterial, b2Body *pBody, sf::Vertex *pQuad);
	~Tile();
	sf::Vector2f getPosition();
	Material *getMaterial();
	b2Body *getBody();
	void setBodyNull();
	void setMaterial(Material *pMaterial);
	void breakNaturally();
	Tile *getRelative(sf::Vector2i pRelativePosition);
private:
	sf::Vector2f mPosition;
	Chunk *mChunk;
	Material *mMaterial;
	b2Body *mBody;
	sf::Vertex *mQuad;
};


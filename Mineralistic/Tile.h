#pragma once

#include <SFML/System/Vector2.hpp>

class Material;
class Chunk;
class b2Body;
class Torch;

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
	void setTorch(Torch *pTorch);
	void breakNaturally();
	void updateLight(float pLightLevel);
	Tile *getRelative(sf::Vector2i pRelativePosition);
	int getLightLevel();
	Torch *getTorch();
private:
	sf::Vector2f mPosition;
	Chunk *mChunk;
	Material *mMaterial;
	b2Body *mBody;
	sf::Vertex *mQuad;
	int mLightLevel; // 0 - 7
	Torch *mTorch;
};


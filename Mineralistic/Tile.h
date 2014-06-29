#pragma once

#include <SFML/System/Vector2.hpp>
#include <vector>

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
	Tile(Chunk *pChunk, sf::Vector2f pPosition, Material *pMaterial, b2Body *pBody, sf::Vertex *pQuad, sf::Vertex *pLightQuad);
	~Tile();
	sf::Vector2f getPosition();
	Material *getMaterial();
	b2Body *getBody();
	void setBodyNull();
	void setMaterial(Material *pMaterial);
	void breakNaturally();
	bool isPositionInMyChunk(sf::Vector2f pPosition);
	Chunk *getChunk();
	Tile *getRelative(sf::Vector2i pRelativePosition);
	Tile *getRelative(int x, int y);
	Torch *getTorch();
	void setLightIntensity(float pIntensity);
	void setTorch(Torch *pTorch);
	float getIntensity();
	sf::Vertex* getQuad() const;
	sf::Vertex* getLightQuad() const;
private:
	float mIntensity;
	sf::Vector2f mPosition;
	Chunk *mChunk;
	Material *mMaterial;
	b2Body *mBody;
	sf::Vertex *mQuad;
	sf::Vertex *mLightQuad;
	Torch *mTorch;
};


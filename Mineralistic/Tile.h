#pragma once

#include <SFML/System/Vector2.hpp>

class Chunk;

class Tile
{
public:
	Tile(Chunk *pChunk, float pHeightValue);
	~Tile();

private:
	Chunk *mChunk;
	float mHeightValue;
	sf::Vector2i mPosition;
};


#pragma once

#include <vector>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "noise/noise.h"
#include "noiseutils.h"

class Chunk;
class ObjectManager;
class TileStop;

class World
{
public:
	World(sf::Vector2i pChunkSize, sf::Vector2i pTileSize, sf::Texture pTileset, ObjectManager *pObjectManager);
	~World();

	void update(float dt);
	void addTileStop(std::string pName, float pHeightStop, sf::Vector2i pPosition);
	TileStop *getTileStopAt(float pHeightValue);

	sf::Texture getTileset();
	std::vector<Chunk*> getLoadedChunks();
	sf::Vector2i getChunkSize();
	sf::Vector2i getTileSize();

private:
	/* Returns a chunk at the given global position. If not chunk is present, then it will call the loadChunk() */
	Chunk *getChunkAt(sf::Vector2f pPosition);

	/* The difference between this one and the above is that this method takes the chunkPosition and not the global position */
	Chunk *getChunk(sf::Vector2i pPosition);

	/* Generates a chunk. Only call this if there are no chunk at this position */
	Chunk *loadChunk(sf::Vector2i pPosition);

private:
	sf::Vector2i mChunkTileSize; // in tiles
	sf::Vector2i mTileSize; // in pixels
	sf::Vector2i mChunkSize; // in pixels (mChunkTileSize * mTileSize)
	std::vector<Chunk*> mChunks;
	std::vector<TileStop*> mTileStops;
	sf::Texture mTileset;
	ObjectManager *mObjectManager;

	noise::module::Perlin mNoiseModule;
};


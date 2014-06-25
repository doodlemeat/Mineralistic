#pragma once

#include <vector>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "noise/noise.h"
#include "noiseutils.h"
#include <mutex>

class Chunk;
class ObjectManager;
class TileStop;
class b2World;
class Material;
class Tile;
class b2Body;

namespace WorldHelper
{
	const float chunk_size = 8.f;
	const float tile_size = 64.f;

	sf::Vector2f toSFMLPositionFromWorldPosition(sf::Vector2f pWorldPosition, bool pCenterOnTile = false);
	sf::Vector2f toWorldPositionFromSFMLPosition(sf::Vector2f pPosition);
	sf::Vector2f toWorldPositionFromChunkPosition(sf::Vector2i pPosition);
	sf::Vector2i chunkPosition(sf::Vector2f pPosition);
	sf::Vector2i toLocalTilePositionFromWorldPosition(sf::Vector2f pPosition);
	sf::Vector2f clampTilePosition(sf::Vector2f pPosition);
}

struct WorldException : public std::exception
{
	WorldException(std::string pMessage) { message = pMessage; }
	~WorldException() throw() {}
	const char* what() const throw() { return message.c_str(); }
	std::string message;

};

class World
{
public:
	enum Direction
	{
		NORTH,
		EAST,
		SOUTH,
		WEST
	};

	World(sf::Vector2i pChunkSize, sf::Vector2i pTileSize, sf::Texture pTileset, ObjectManager *pObjectManager, b2World *pB2World);
	~World();

	void update(float dt);
	void addTileStop(std::string pName, float pHeightStop);
	void registerMaterial(Material *pMaterial);

	b2Body *createBody(sf::Vector2f pPosition);
	TileStop *getTileStopAt(float pHeightValue);

	sf::Texture getTileset();
	std::vector<Chunk*> getLoadedChunks();
	sf::Vector2i getChunkSize();
	sf::Vector2i getTileSize(); 
	Material *getMaterial(std::string pName);

	b2Body *createLine(sf::Vector2f from, sf::Vector2f to);
	b2Body *createChain(sf::Vector2f *pVertices, int pLength);

	Chunk *getChunkByChunkPosition(sf::Vector2i pPosition);
	Chunk *getChunkByWorldPosition(sf::Vector2f pPosition);
	Chunk *loadChunk(sf::Vector2i pPosition);

	Tile *getTileByWorldPosition(sf::Vector2f pPosition);
	Tile *getClosestTileInDirection(sf::Vector2f pPosition, Direction direction, unsigned int length);
	std::vector<Material*> getLumpables(float pHeightLimit);

	void processNeighborLight(Tile *pCurrent, int pLightLevel, int *pIterationCount);

private:
	sf::Vector2i mChunkTileSize; // in tiles
	sf::Vector2i mTileSize; // in pixels
	sf::Vector2i mChunkSize; // in pixels (mChunkTileSize * mTileSize)
	std::vector<Chunk*> mChunks;
	std::vector<TileStop*> mTileStops;
	std::vector<Material*> mMaterials; // Stores all materials
	std::vector<Material*> mLumpables; // Stores only lumpable materials

	sf::Texture mTileset;
	ObjectManager *mObjectManager;
	b2World *mB2World;

	noise::module::Perlin mNoiseModule;
};

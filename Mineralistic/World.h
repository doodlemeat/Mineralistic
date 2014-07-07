#pragma once

#include <vector>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "noise/noise.h"
#include "noiseutils.h"
#include "Thor/Particles/ParticleSystem.hpp"
#include "Thor/Particles/Emitters.hpp"

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
	sf::Vector2i toChunkPositionFromWorldPosition(sf::Vector2f pPosition);
	sf::Vector2i toLocalTilePositionFromWorldPosition(sf::Vector2f pPosition);
	sf::Vector2f clampTilePosition(sf::Vector2f pPosition);
	float worldDistanceBetweenSFMLPos(const sf::Vector2f &p1, const sf::Vector2f &p2);
}

namespace thor
{
	class Timer;
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

	World(sf::Texture pTileset, ObjectManager *pObjectManager, b2World *pB2World);
	~World();

	void update(float dt);
	void addTileStop(std::string pName, float pHeightStop);
	void registerMaterial(Material *pMaterial);

	b2Body *createBody(sf::Vector2f pPosition);
	TileStop *getTileStopAt(float pHeightValue);

	sf::Texture getTileset();
	std::vector<Chunk*> getLoadedChunks();
	Material *getMaterial(std::string pName);

	b2Body *createLine(sf::Vector2f from, sf::Vector2f to);
	b2Body *createChain(sf::Vector2f *pVertices, int pLength);

	Chunk *getChunkByChunkPosition(sf::Vector2i pPosition);
	Chunk *getChunkByWorldPosition(sf::Vector2f pPosition);
	Chunk *loadChunk(sf::Vector2i pPosition);

	Tile *getTileByWorldPosition(sf::Vector2f pPosition);
	Tile *getClosestTileInDirection(sf::Vector2f pPosition, Direction direction, unsigned int length);
	std::vector<Material*> getLumpables(float pHeightLimit);
	thor::ParticleSystem *getBlockParticleSystem();
	thor::UniversalEmitter &getBlockParticleEmitter();
	ObjectManager *getObjectManager() const;

	const int & getSeed();
	const int & getOctaveCount();
	const int & getNoiseQuality();
	const double & getFrequency();
	void spawnMonsters();
	std::vector<Material*> &getMaterialList();
	bool isChunkLoadedHere(sf::Vector2f pWorldPosition);
private:
	std::vector<Chunk*> mChunks;

	std::vector<TileStop*> mTileStops;
	std::vector<Material*> mMaterials; // Stores all materials
	std::vector<Material*> mLumpables; // Stores only lumpable materials

	sf::Texture mTileset;
	ObjectManager *mObjectManager;
	b2World *mB2World;

	int seed;
	int octaveCount;
	int noise_quality;
	double frequency;

	noise::module::Perlin mNoiseModule;

	thor::Timer* mMonsterSpawnerTimer;
	thor::ParticleSystem *mBlockParticleSystem;
	thor::UniversalEmitter mBlockParticleEmitter;
};


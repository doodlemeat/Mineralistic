#include "World.h"
#include "ObjectManager.h"
#include "SFML/Graphics.hpp"
#include "GameObject.h"
#include "Chunk.h"
#include "Thor/Math/Random.hpp"
#include <climits>
#include <iostream>
#include "TileStop.h"
#include "Logger.h"
#include "Material.h"
#include "PhysicsScale.h"
#include "Box2D/Box2D.h"
#include "Tile.h"
#include "Math.h"
#include "Thor/Time/Timer.hpp"
#include "ObjectGroup.h"
#include "Player.h"

World::World(sf::Vector2i pChunkSize, sf::Vector2i pTileSize, sf::Texture pTileset, ObjectManager *pObjectManager, b2World *pB2World)
{
	mTileSize = pTileSize;
	mChunkTileSize = pChunkSize;
	mChunkSize = sf::Vector2i(mTileSize.x * mChunkTileSize.x, mTileSize.y * mChunkTileSize.y);
	mTileset = pTileset;
	mObjectManager = pObjectManager;
	mB2World = pB2World;
	mMonsterSpawnerTimer = new thor::Timer();
	mMonsterSpawnerTimer->restart(sf::seconds(1));

	seed = thor::random(INT_MIN, INT_MAX);
	frequency = 0.2; // (double)thor::random(0.01f, 0.08f);
	octaveCount = 1; // thor::random(1, noise::module::PERLIN_MAX_OCTAVE);
	noise_quality = noise::QUALITY_FAST;

	mNoiseModule.SetFrequency(frequency);
	mNoiseModule.SetOctaveCount(octaveCount);
	mNoiseModule.SetSeed(seed);
	mNoiseModule.SetNoiseQuality((noise::NoiseQuality)noise_quality);
}

World::~World()
{
	auto it = mChunks.begin();
	while (it != mChunks.end())
	{
		delete (*it);
		(*it) = nullptr;
		++it;
	}

	auto itTileStop = mTileStops.begin();
	while (itTileStop != mTileStops.end())
	{
		delete (*itTileStop);
		(*itTileStop) = nullptr;
		++itTileStop;
	}

	auto itMaterial = mMaterials.begin();
	while (itMaterial != mMaterials.end())
	{
		delete (*itMaterial);
		(*itMaterial) = nullptr;
		++itMaterial;
	}

	delete mMonsterSpawnerTimer;
}

void World::update(float dt)
{
	// Check if we can load more chunks
	sf::Vector2f playerPosition = WorldHelper::toWorldPositionFromSFMLPosition(mObjectManager->getObject("Player")->getSprite()->getPosition());

	Chunk *playerChunk = getChunkByWorldPosition(playerPosition);

	sf::Vector2i playerChunkPosition = playerChunk->getPosition();

	sf::Vector2i NW(playerChunkPosition.x - 1, playerChunkPosition.y - 1);   // north west
	sf::Vector2i N (playerChunkPosition.x    , playerChunkPosition.y - 1);   // north
	sf::Vector2i NE(playerChunkPosition.x + 1, playerChunkPosition.y - 1);   // nort east
	sf::Vector2i W (playerChunkPosition.x - 1, playerChunkPosition.y);       // west
	sf::Vector2i E (playerChunkPosition.x + 1, playerChunkPosition.y);       // east
	sf::Vector2i SW(playerChunkPosition.x - 1, playerChunkPosition.y + 1);   // south west
	sf::Vector2i S (playerChunkPosition.x    , playerChunkPosition.y + 1);   // south
	sf::Vector2i SE(playerChunkPosition.x + 1, playerChunkPosition.y + 1);   // south east

	sf::Vector2i NNWW(playerChunkPosition.x - 2, playerChunkPosition.y - 2); // north north west west
	sf::Vector2i NNW(playerChunkPosition.x - 1, playerChunkPosition.y - 2);  // north north west
	sf::Vector2i NN(playerChunkPosition.x, playerChunkPosition.y - 2);       // north north
	sf::Vector2i NNE(playerChunkPosition.x + 1, playerChunkPosition.y - 2);  // north north east
	sf::Vector2i NNEE(playerChunkPosition.x + 2, playerChunkPosition.y - 2); // north north east east

	sf::Vector2i WWN(playerChunkPosition.x - 2, playerChunkPosition.y - 1);  // west west north
	sf::Vector2i WW(playerChunkPosition.x - 2, playerChunkPosition.y);       // west west
	sf::Vector2i WWS(playerChunkPosition.x - 2, playerChunkPosition.y + 1);  // west west south

	sf::Vector2i SSWW(playerChunkPosition.x - 2, playerChunkPosition.y + 2); // south south west west
	sf::Vector2i SSW(playerChunkPosition.x - 1, playerChunkPosition.y + 2);  // south south west
	sf::Vector2i SS(playerChunkPosition.x, playerChunkPosition.y + 2);       // south south
	sf::Vector2i SSE(playerChunkPosition.x + 1, playerChunkPosition.y + 2);  // south south east
	sf::Vector2i SSEE(playerChunkPosition.x + 2, playerChunkPosition.y + 2); // south south east east

	sf::Vector2i EEN(playerChunkPosition.x + 2, playerChunkPosition.y - 1);  // east east north
	sf::Vector2i EE(playerChunkPosition.x + 2, playerChunkPosition.y);       // east east
	sf::Vector2i EES(playerChunkPosition.x + 2, playerChunkPosition.y + 1);  // east east south

	getChunkByChunkPosition(NW);
	getChunkByChunkPosition(N);
	getChunkByChunkPosition(NE);
	getChunkByChunkPosition(W);
	getChunkByChunkPosition(E);
	getChunkByChunkPosition(SW);
	getChunkByChunkPosition(S);
	getChunkByChunkPosition(SE);

	getChunkByChunkPosition(NNWW);
	getChunkByChunkPosition(NNW);
	getChunkByChunkPosition(NN);
	getChunkByChunkPosition(NNE);
	getChunkByChunkPosition(NNEE);

	getChunkByChunkPosition(WWN);
	getChunkByChunkPosition(WW);
	getChunkByChunkPosition(WWS);

	getChunkByChunkPosition(SSWW);
	getChunkByChunkPosition(SSW);
	getChunkByChunkPosition(SS);
	getChunkByChunkPosition(SSE);
	getChunkByChunkPosition(SSEE);

	getChunkByChunkPosition(EEN);
	getChunkByChunkPosition(EE);
	getChunkByChunkPosition(EES);

	spawnMonsters();
}

Chunk *World::getChunkByChunkPosition(sf::Vector2i pPosition)
{
	for (auto &chunk : mChunks)
	{
		if (chunk->isPosition(pPosition))
		{
			return chunk;
		}
	}
	return loadChunk(pPosition);
}

Chunk *World::getChunkByWorldPosition(sf::Vector2f pPosition)
{
	sf::Vector2i chunkPosition = WorldHelper::toChunkPositionFromWorldPosition(pPosition);
	return getChunkByChunkPosition(chunkPosition);
}

Chunk *World::loadChunk(sf::Vector2i pPosition)
{
	sf::FloatRect bounds(WorldHelper::toWorldPositionFromChunkPosition(pPosition), sf::Vector2f((float)mChunkTileSize.x, (float)mChunkTileSize.y));
	noise::utils::NoiseMap heightMap;
	noise::utils::NoiseMapBuilderPlane heightMapBuilder;
	heightMapBuilder.SetSourceModule(mNoiseModule);
	heightMapBuilder.SetDestNoiseMap(heightMap);
	heightMapBuilder.SetDestSize(mChunkTileSize.x, mChunkTileSize.y);
	heightMapBuilder.SetBounds(bounds.left, bounds.left + bounds.width - 1, bounds.top, bounds.top + bounds.height - 1);
	heightMapBuilder.Build();

	Chunk *chunk = new Chunk(this, mB2World);
	chunk->setPosition(pPosition);
	mChunks.push_back(chunk);
	chunk->buildChunk(&heightMap);
	chunk->setTexture(&mTileset);
	return chunk;
}

sf::Texture World::getTileset()
{
	return mTileset;
}

std::vector<Chunk*> World::getLoadedChunks()
{
	return mChunks;
}

sf::Vector2i World::getChunkSize()
{
	return mChunkSize;
}

sf::Vector2i World::getTileSize()
{
	return mTileSize;
}

void World::addTileStop(std::string pName, float pHeightStop)
{
	Material *material = getMaterial(pName);
	if (material == nullptr)
	{
		return;
	}

	TileStop *tileStop = new TileStop(material);
	tileStop->setHeightStop(pHeightStop);
	mTileStops.push_back(tileStop);
	std::cout << "Added tilestop at " << pHeightStop << " to material " << pName << std::endl;
}

TileStop *World::getTileStopAt(float pHeightValue)
{
	float lastCap = -1.f;
	for (std::size_t i = 0; i < mTileStops.size(); i++)
	{
		if (pHeightValue >= lastCap && pHeightValue <= mTileStops[i]->getHeightStop())
		{
			return mTileStops[i];
		}
		lastCap = mTileStops[i]->getHeightStop();
	}
	return mTileStops.back();
}

void World::registerMaterial(Material *pMaterial)
{
	for (auto &material : mMaterials)
	{
		if (material->getName() == pMaterial->getName())
		{
			std::cout << "Failed to add material " << pMaterial->getName() << ". Material already exist" << std::endl;
			break;
			return;
		}
	}
	std::cout << "Registred material " << pMaterial->getName() << std::endl;

	if (pMaterial->isLumpable())
	{
		mLumpables.push_back(pMaterial);
	}
	mMaterials.push_back(pMaterial);
}

Material * World::getMaterial(std::string pName)
{
	for (auto &material : mMaterials)
	{
		if (material->getName() == pName)
		{
			return material;
		}
	}

	std::cout << "Failed to find material with name " << pName << ". The material does not exist." << std::endl;
	return nullptr;
}

b2Body *World::createBody(sf::Vector2f pPosition)
{
	b2BodyDef bodyDef;
	bodyDef.angle = 0;
	bodyDef.type = b2_staticBody;
	b2Vec2 position = PhysicsScale::gameToPhys(pPosition);
	bodyDef.position.Set(position.x, position.y);

	b2Body *body = mB2World->CreateBody(&bodyDef);

	b2PolygonShape boxShape;
	sf::Vector2f boundingBox(32, 32);
	b2Vec2 boundingBoxPhysic = PhysicsScale::gameToPhys(boundingBox);
	boxShape.SetAsBox(boundingBoxPhysic.x, boundingBoxPhysic.y);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1;
	fixtureDef.friction = 0;
	body->CreateFixture(&fixtureDef);
	return body;
}

b2Body *World::createLine(sf::Vector2f from, sf::Vector2f to)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;

	float centerX = PhysicsScale::gameToPhys((from.x + to.x) / 2.f);
	float centerY = PhysicsScale::gameToPhys((from.y + to.y) / 2.f);

	float length = PhysicsScale::gameToPhys(std::sqrtf((from.x - to.x) * (from.x - to.x) + (from.y - to.y) * (from.y - to.y)));

	bodyDef.position.Set(centerX, centerY);
	bodyDef.angle = 0;

	b2Body *body = mB2World->CreateBody(&bodyDef);

	b2FixtureDef fixtureDef;
	fixtureDef.density = 1;
	fixtureDef.friction = 0;

	b2EdgeShape shape;
	shape.Set(b2Vec2(-length / 2.f, 0), b2Vec2(length / 2.f, 0));
	fixtureDef.shape = &shape;

	body->CreateFixture(&fixtureDef);
	body->SetTransform(b2Vec2(centerX, centerY), std::atan2f(to.y - from.y, to.x - from.x));
	return body;
}

b2Body *World::createChain(sf::Vector2f *pVertices, int pLength)
{
	b2BodyDef bodyDef;
	bodyDef.position.Set(0.f, 0.f);
	b2Body *body = mB2World->CreateBody(&bodyDef);

	b2ChainShape shape;

	b2Vec2 vertices[4];

	for (int i = 0; i < pLength; i++)
	{
		vertices[i] = PhysicsScale::gameToPhys(pVertices[i]);
	}
	shape.CreateLoop(vertices, 4);

	b2FixtureDef fixtureDef;
	fixtureDef.density = 1;
	fixtureDef.shape = &shape;

	body->CreateFixture(&fixtureDef);

	return body;
}

Tile *World::getClosestTileInDirection(sf::Vector2f pPosition, Direction direction, unsigned int length)
{
	// Get start tile
	Tile *startTile = getTileByWorldPosition(pPosition);
	std::cout << "startTile ";
	Logger::vector2(startTile->getPosition());

	// Setup a few variables to be used in the algorithm
	float offsetX = pPosition.x;
	float offsetY = pPosition.y;
	
	int offset = 0;
	int cap = 0;

	// Apprently, there are some special cases for negative world position coordinates
	switch (direction)
	{
	case NORTH:
		cap = offsetY - length;
		offset = offsetY;

		if (offsetY < 0) cap--;
		for (int i = offset - 1; i >= cap; i--)
		{
			Tile *next = getTileByWorldPosition(sf::Vector2f(offsetX, static_cast<float>(i)));
			if (next->getMaterial()->isCollidable())
			{
				std::cout << "found tile at ";
				Logger::vector2(next->getPosition());
				return next;
			}
		}
		break;
	case EAST: // TODO
		cap = offsetX + length;
		offset = offsetX;
		break;
	case SOUTH: // TODO
		cap = offsetY + length;
		offset = offsetY;
		break;
	case WEST:
		cap = offsetX - length;
		offset = static_cast<int>(offsetX);

		if (offsetX < 0) cap--;
		for (int i = offset - 1; i >= cap; i--)
		{
			Tile *next = getTileByWorldPosition(sf::Vector2f(static_cast<float>(i), offsetY));
			if (next->getMaterial()->isCollidable())
			{
				return next;
			}
		}
		break;
	}

	throw WorldException("Failed to find tile");
}

Tile *World::getTileByWorldPosition(sf::Vector2f pPosition)
{
	Chunk* chunk = getChunkByWorldPosition(pPosition);
	return chunk->getTileAt(WorldHelper::clampTilePosition(pPosition));
}

std::vector<Material*> World::getLumpables(float pHeightLimit)
{
	std::vector<Material*> lumpables;
	for (std::size_t i = 0; i < mLumpables.size(); i++)
	{
		if (pHeightLimit >= mLumpables[i]->getUpperLimitY())
		{
			lumpables.push_back(mLumpables[i]);
		}
	}
	return lumpables;
}

const int & World::getSeed()
{
	return seed;
}

const int & World::getOctaveCount()
{
	return octaveCount;
}

const int & World::getNoiseQuality()
{
	return noise_quality;
}

const double & World::getFrequency()
{
	return frequency;
}

void World::spawnMonsters()
{
	if (mObjectManager->getGroup("monsters")->getObjects()->size() == 0) return;
	if (mMonsterSpawnerTimer->isExpired())
	{
		std::cout << "Spawning a monster" << std::endl;
		// Try to spawn a monster
		Player *player = static_cast<Player*>(mObjectManager->getObject("Player"));

		sf::Vector2f playerPosition = WorldHelper::toWorldPositionFromSFMLPosition(mObjectManager->getObject("Player")->getSprite()->getPosition());
		Chunk *playerChunk = getChunkByWorldPosition(playerPosition);

		sf::Vector2i avaibleChunkPositions[64];
		int size = 0;

		int threshhold = 3;
		int minDist = 2;

		for (int x = -threshhold; x <= threshhold; x++)
		{
			for (int y = -threshhold; y <= threshhold; y++)
			{
				if ((x < -minDist || x > minDist) || (y < -minDist || y > minDist))
				{
					avaibleChunkPositions[size] = sf::Vector2i(x, y);
					size++;
				}
			}
		}

		int randomChunkPositionIndex = thor::random(0, size);
		Chunk *chunk = playerChunk->getRelative(avaibleChunkPositions[randomChunkPositionIndex]);
		Logger::vector2(avaibleChunkPositions[randomChunkPositionIndex]);
		Logger::vector2(chunk->getPosition());

		Tile* avaibleTiles[64];
		size = 0;
		for (int x = 0; x < 8; x++)
		{
			for (int y = 0; y < 8; y++)
			{
				Tile *tile = chunk->getTile(x, y);
				if (!tile->getMaterial()->isCollidable() && tile->getRelative(0, 1)->getMaterial()->isCollidable())
				{
					avaibleTiles[size] = tile;
					size++;
				}
			}
		}
		
		mMonsterSpawnerTimer->restart(sf::seconds(1));
		if (size == 0) return;

		Tile *chosenTile = avaibleTiles[thor::random(0, size - 1)];

		mObjectManager->spawnMonster(chosenTile->getPosition());
	}
}

namespace WorldHelper
{
	sf::Vector2f toWorldPositionFromSFMLPosition(sf::Vector2f pPosition)
	{
		sf::Vector2f worldPosition = pPosition / tile_size;
		return worldPosition;
	}

	sf::Vector2f toWorldPositionFromChunkPosition(sf::Vector2i pPosition)
	{
		sf::Vector2f worldPosition(0, 0);
		worldPosition.x = pPosition.x * chunk_size;
		worldPosition.y = pPosition.y * chunk_size;
		return worldPosition;
	}

	sf::Vector2i toChunkPositionFromWorldPosition(sf::Vector2f pPosition)
	{
		sf::Vector2i chunkPos(0, 0);
		chunkPos.x = static_cast<int>(std::floor(pPosition.x / chunk_size));
		chunkPos.y = static_cast<int>(std::floor(pPosition.y / chunk_size));
		return chunkPos;
	}

	sf::Vector2i toLocalTilePositionFromWorldPosition(sf::Vector2f pPosition)
	{
		sf::Vector2i tilePosition(0, 0);
		tilePosition.x = static_cast<int>(std::fmod(pPosition.x, chunk_size));
		tilePosition.y = static_cast<int>(std::fmod(pPosition.y, chunk_size));
		if (tilePosition.x < 0)
		{
			tilePosition.x = std::abs(tilePosition.x);
			tilePosition.x = chunk_size - tilePosition.x;
		}
		if (tilePosition.y < 0)
		{
			tilePosition.y = std::abs(tilePosition.y);
			tilePosition.y = chunk_size - tilePosition.y;
		}
		return tilePosition;
	}

	sf::Vector2f clampTilePosition(sf::Vector2f pPosition)
	{
		float xRemainder = std::fmod(pPosition.x, 8.f);
		if (xRemainder < 0.f) xRemainder += 8.f;
		float yRemainder = std::fmod(pPosition.y, 8.f);
		if (yRemainder < 0.f) yRemainder += 8.f;

		return sf::Vector2f(xRemainder, yRemainder);
	}

	sf::Vector2f toSFMLPositionFromWorldPosition(sf::Vector2f pWorldPosition, bool pCenterOnTile /*= false*/)
	{
		sf::Vector2f SFMLPosition = pWorldPosition * tile_size;
		if (pCenterOnTile)
		{
			SFMLPosition.x += tile_size / 2.f;
			SFMLPosition.y += tile_size / 2.f;
		}
		return SFMLPosition;
	}
}
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

World::World(sf::Vector2i pChunkSize, sf::Vector2i pTileSize, sf::Texture pTileset ,ObjectManager *pObjectManager)
{
	mTileSize = pTileSize;
	mChunkTileSize = pChunkSize;
	mChunkSize = sf::Vector2i(mTileSize.x * mChunkTileSize.x, mTileSize.y * mChunkTileSize.y);
	mTileset = pTileset;
	mObjectManager = pObjectManager;

	mNoiseModule.SetFrequency(0.1);
	mNoiseModule.SetSeed(thor::random(INT_MIN, INT_MAX));
	mNoiseModule.SetNoiseQuality(noise::QUALITY_BEST);
}

World::~World()
{
}

void World::update(float dt)
{
	// Check if we can load more chunks
	sf::Vector2f playerPosition = mObjectManager->getObject("Player")->getSprite()->getPosition();
	Chunk *playerChunk = getChunkAt(playerPosition);

	// We always want at least one chunk loaded from the player chunk in all 8 directions, like this pattern
	/*
		C C C
		C P C
		C C C
	*/
	sf::Vector2i playerChunkPosition = playerChunk->getPosition();

	sf::Vector2i NW(playerChunkPosition.x - 1, playerChunkPosition.y - 1); // north west
	sf::Vector2i N (playerChunkPosition.x    , playerChunkPosition.y - 1); // north
	sf::Vector2i NE(playerChunkPosition.x + 1, playerChunkPosition.y - 1); // nort east
	sf::Vector2i W (playerChunkPosition.x - 1, playerChunkPosition.y);     // west
	sf::Vector2i E (playerChunkPosition.x + 1, playerChunkPosition.y);     // east
	sf::Vector2i SW(playerChunkPosition.x - 1, playerChunkPosition.y + 1); // south west
	sf::Vector2i S (playerChunkPosition.x    , playerChunkPosition.y + 1); // south
	sf::Vector2i SE(playerChunkPosition.x + 1, playerChunkPosition.y + 1); // south east

	getChunk(NW);
	getChunk(N);
	getChunk(NE);
	getChunk(W);
	getChunk(E);
	getChunk(SW);
	getChunk(S);
	getChunk(SE);
}

Chunk *World::getChunkAt(sf::Vector2f pPosition)
{
	sf::Vector2i chunkPosition(static_cast<int>(std::floor(pPosition.x / mChunkSize.x)), static_cast<int>(std::floor(pPosition.y / mChunkSize.y)));
	for (auto &chunk : mChunks)
	{
		if (chunk->isPosition(chunkPosition))
		{
			return chunk;
		}
	}

	return loadChunk(chunkPosition);
}

Chunk *World::getChunk(sf::Vector2i pPosition)
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

Chunk *World::loadChunk(sf::Vector2i pPosition)
{
	sf::Vector2i chunkPos = pPosition;
	pPosition.x *= mChunkTileSize.x;
	pPosition.y *= mChunkTileSize.y;
	sf::FloatRect bounds(static_cast<sf::Vector2f>(pPosition), sf::Vector2f(static_cast<float>(mChunkTileSize.x), static_cast<float>(mChunkTileSize.y)));

	noise::utils::NoiseMap heightMap;
	noise::utils::NoiseMapBuilderPlane heightMapBuilder;
	heightMapBuilder.SetSourceModule(mNoiseModule);
	heightMapBuilder.SetDestNoiseMap(heightMap);
	heightMapBuilder.SetDestSize(mChunkTileSize.x, mChunkTileSize.y);
	heightMapBuilder.SetBounds(bounds.left, bounds.left + bounds.width - 1, bounds.top, bounds.top + bounds.height - 1);
	heightMapBuilder.Build();

	Chunk *chunk = new Chunk(this);
	chunk->setPosition(chunkPos);
	chunk->buildChunk(&heightMap);
	chunk->setTexture(&mTileset);
	mChunks.push_back(chunk);
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

void World::addTileStop(std::string pName, float pHeightStop, sf::Vector2i pPosition)
{
	TileStop *tileStop = new TileStop(pName);
	tileStop->setTexturePosition(pPosition);
	tileStop->setHeightStop(pHeightStop);
	mTileStops.push_back(tileStop);
}

TileStop *World::getTileStopAt(float pHeightValue)
{
	for (auto &stop : mTileStops)
	{
		if (pHeightValue <= stop->getHeightStop())
		{
			return stop;
		}
	}
	return mTileStops.back();
}

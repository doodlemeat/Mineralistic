#include "Chunk.h"
#include "SFML\Graphics.hpp"
#include "World.h"
#include <iostream>
#include "Tile.h"
#include "noiseutils.h"
#include "noise/noise.h"
#include "Logger.h"
#include "TileStop.h"
#include "Box2D/Box2D.h"
#include "Material.h"
#include "Thor/Math/Random.hpp"

Chunk::Chunk(World *pWorld, b2World *pB2World)
{
	mWorld = pWorld;
	mB2World = pB2World;
	mChunkBorder.setOutlineThickness(4);
	mChunkBorder.setSize(sf::Vector2f(8 * 64, 8 * 64));
	mChunkBorder.setOutlineColor(sf::Color::White);
	mChunkBorder.setFillColor(sf::Color::Transparent);
}

Chunk::~Chunk()
{
	auto iterTile = mTiles.begin();
	while (iterTile != mTiles.end())
	{
		auto it = (*iterTile).begin();
		while (it != (*iterTile).end())
		{
			Tile *tile = (*it);
			delete tile;
			tile = nullptr;
			++it;
		}
		++iterTile;
	}
}

bool Chunk::isPosition(sf::Vector2i pPosition)
{
	return mPosition == pPosition;
}

void Chunk::setPosition(sf::Vector2i pPosition)
{
	mPosition = pPosition;
	mChunkBorder.setPosition(WorldHelper::toSFMLPositionFromWorldPosition(WorldHelper::toWorldPositionFromChunkPosition(mPosition)));
}

void Chunk::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	// apply the tileset texture
	states.texture = &mTexture;

	// draw the vertex array
	target.draw(mVertices, states);
}

void Chunk::buildChunk(noise::utils::NoiseMap *pHeightMap)
{
	// Resize the tiles space
	mTiles.resize(pHeightMap->GetWidth());
	for (std::size_t x = 0; x < mTiles.size(); x++)
	{
		mTiles[x].resize(pHeightMap->GetHeight());
	}

	// Set vertices type and size
	mVertices.setPrimitiveType(sf::Quads);
	mVertices.resize(pHeightMap->GetWidth() * pHeightMap->GetWidth() * 4);

	// Get the offset position of all tiles position
	sf::Vector2i tileSize = mWorld->getTileSize();
	sf::Vector2i chunkSize = mWorld->getChunkSize();
	sf::Vector2f offsetPositon = sf::Vector2f(mPosition);
	offsetPositon.x *= chunkSize.x;
	offsetPositon.y *= chunkSize.y;

	// Build tiles
	for (std::size_t x = 0; x < mTiles.size(); x++)
	{
		for (std::size_t y = 0; y < mTiles[x].size(); y++)
		{
			// Sometimes libnoise can return a value over 1.0, better be sure to cap the top and bottom..
			float heightValue = pHeightMap->GetValue(x, y);
			if (heightValue > 1.f) heightValue = 1.f;
			if (heightValue < -1.f) heightValue = -1.f;

			// Get a pointer to the current tile's quad
			sf::Vertex *quad = &mVertices[(x + y * pHeightMap->GetWidth()) * 4];

			quad[0].position = sf::Vector2f(offsetPositon.x + x * tileSize.x, offsetPositon.y + y * tileSize.y);
			quad[1].position = sf::Vector2f(offsetPositon.x + (x + 1) * tileSize.x, offsetPositon.y + y * tileSize.y);
			quad[2].position = sf::Vector2f(offsetPositon.x + (x + 1) * tileSize.x, offsetPositon.y + (y + 1) * tileSize.y);
			quad[3].position = sf::Vector2f(offsetPositon.x + x * tileSize.x, offsetPositon.y + (y + 1) * tileSize.y);

			TileStop *tilestop = mWorld->getTileStopAt(heightValue);
			sf::FloatRect textureRect = static_cast<sf::FloatRect>(tilestop->getMaterial()->getTextureRect());

			// find out which type of tile to render
			Material *material = tilestop->getMaterial();

			// define its 4 texture coordinates
			quad[0].texCoords = sf::Vector2f(textureRect.left, textureRect.top) + sf::Vector2f(0.5f, 0.5f);
			quad[1].texCoords = sf::Vector2f(textureRect.left + textureRect.width, textureRect.top) + sf::Vector2f(-0.5f, 0.5f);
			quad[2].texCoords = sf::Vector2f(textureRect.left + textureRect.width, textureRect.top + textureRect.height) + sf::Vector2f(-0.5f, -0.5f);
			quad[3].texCoords = sf::Vector2f(textureRect.left, textureRect.top + textureRect.height) + sf::Vector2f(0.5f, -0.5f);

			quad[0].color = sf::Color(255, 255, 255, 100);
			quad[1].color = sf::Color(255, 255, 255, 100);
			quad[2].color = sf::Color(255, 255, 255, 100);
			quad[3].color = sf::Color(255, 255, 255, 100);

			// create a physics body
			b2Body *body = nullptr;
			if (tilestop->getMaterial()->isCollidable())
			{
				sf::Vector2f vertices[4];
				vertices[0] = quad[0].position;
				vertices[1] = quad[1].position;
				vertices[2] = quad[2].position;
				vertices[3] = quad[3].position;
				body = mWorld->createChain(vertices, 4);
			}

			sf::Vector2f worldPosition = WorldHelper::toWorldPositionFromChunkPosition(mPosition) + sf::Vector2f(x, y);

			// Instantiate a new Tile object with the noise value, this doesn't do anything yet..
			mTiles[x][y] = new Tile(this, worldPosition, material, body, quad);
		}
	}

	// Get a list of avaible lumpables
	sf::Vector2f worldPos = WorldHelper::toWorldPositionFromChunkPosition(mPosition);
	std::vector<Material*> lumpables = mWorld->getLumpables(worldPos.y);
	if (lumpables.size() > 0)
	{
		int maxLumps = thor::random(1, 3);

		for (int i = 0; i < maxLumps; i++)
		{
			int x = thor::random(0, 7);
			int y = thor::random(0, 7);
			if (!mTiles[x][y]->getMaterial()->isCollidable()) continue;
			int randomIndex = thor::random(0, static_cast<int>(lumpables.size() - 1));
			Material* material = lumpables[randomIndex];
			int lumpSize = thor::random(material->getMinLumpSize(), material->getMaxLumpSize()) - 1;
			mTiles[x][y]->setMaterial(material);
			Tile *usedTiles[10];
			for (int j = 0; j < lumpSize; j++)
			{

			}
		}
	}
}

void Chunk::setTexture(sf::Texture *pTexture)
{
	mTexture = *pTexture;
}

sf::Vector2i Chunk::getPosition()
{
	return mPosition;
}

Tile *Chunk::getTileAt(sf::Vector2f pPosition)
{
	sf::Vector2i tilePosition = WorldHelper::toLocalTilePositionFromWorldPosition(pPosition);
	return mTiles[tilePosition.x][tilePosition.y];
}

Tile *Chunk::getTile(sf::Vector2i pPosition)
{
	return mTiles[pPosition.x][pPosition.y];
}

World *Chunk::getWorld()
{
	return mWorld;
}

b2World *Chunk::getB2World()
{
	return mB2World;
}

sf::Vertex* Chunk::getVertices(sf::Vector2f pWorldPosition)
{
	sf::Vector2i tilePos = WorldHelper::toLocalTilePositionFromWorldPosition(pWorldPosition);
	return &mVertices[(tilePos.x + tilePos.y * 8) * 4];
}

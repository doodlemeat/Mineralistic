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

	// Allocate space for tiles
	mTiles = new Tile**[8];
	for (int i = 0; i < 8; i++)
	{
		mTiles[i] = new Tile*[8];
	}

	// Set default value of nullptr
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			mTiles[x][y] = nullptr;
		}
	}
}

Chunk::~Chunk()
{
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			delete mTiles[x][y];
		}
		delete[] mTiles[x];
	}
	delete[] mTiles;
}

bool Chunk::isPosition(sf::Vector2i pPosition)
{
	return mPosition == pPosition;
}

void Chunk::setPosition(sf::Vector2i pPosition)
{
	mPosition = pPosition;
}

void Chunk::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.texture = &mTexture;
	target.draw(mVertices, states);
}

void Chunk::buildChunk(noise::utils::NoiseMap *pHeightMap)
{
	// Set vertices type and size
	mVertices.setPrimitiveType(sf::Quads);
	mVertices.resize(8 * 8 * 4);

	// Get the offset position of all tiles position
	sf::Vector2f offsetPositon = sf::Vector2f(mPosition);
	offsetPositon.x *= 64 * 8;
	offsetPositon.y *= 64 * 8;

	// Build tiles
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			sf::Vector2f worldPosition = WorldHelper::toWorldPositionFromChunkPosition(mPosition) + sf::Vector2f(x, y);

			// Sometimes libnoise can return a value over 1.0, better be sure to cap the top and bottom..
			float heightValue = pHeightMap->GetValue(x, y);
			if (heightValue > 1.f) heightValue = 1.f;
			if (heightValue < -1.f) heightValue = -1.f;

			// Get a pointer to the current tile's quad
			sf::Vertex *quad = &mVertices[(x + y * pHeightMap->GetWidth()) * 4];

			quad[0].position = sf::Vector2f(offsetPositon.x + x * 64, offsetPositon.y + y * 64);
			quad[1].position = sf::Vector2f(offsetPositon.x + (x + 1) * 64, offsetPositon.y + y * 64);
			quad[2].position = sf::Vector2f(offsetPositon.x + (x + 1) * 64, offsetPositon.y + (y + 1) * 64);
			quad[3].position = sf::Vector2f(offsetPositon.x + x * 64, offsetPositon.y + (y + 1) * 64);

			Material *material = nullptr;
			if (worldPosition.y <= -5)
			{
				material = mWorld->getMaterial("Sky");
			}
			else if(worldPosition.y == -4)
			{
				material = mWorld->getMaterial("Grass");
			}
			else if (worldPosition.y >= -3 && worldPosition.y <= 0)
			{
				material = mWorld->getMaterial("Dirt");
			}
			else
			{
				material = mWorld->getTileStopAt(heightValue)->getMaterial();
			}
			sf::FloatRect textureRect = static_cast<sf::FloatRect>(material->getTextureRect());

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
			if (material->isCollidable())
			{
				sf::Vector2f vertices[4];
				vertices[0] = quad[0].position;
				vertices[1] = quad[1].position;
				vertices[2] = quad[2].position;
				vertices[3] = quad[3].position;
				body = mWorld->createChain(vertices, 4);
			}

			// Instantiate a new Tile object with the noise value, this doesn't do anything yet..
			mTiles[x][y] = new Tile(this, worldPosition, material, body, quad);
		}
	}
	
	// Get a list of avaible lumpables
	int iter = 0;
	sf::Vector2f worldPos = WorldHelper::toWorldPositionFromChunkPosition(mPosition);
	std::vector<Material*> lumpables = mWorld->getLumpables(worldPos.y);
	if (lumpables.size() > 0)
	{
		int maxLumps = thor::random(0, 1);

		for (int i = 0; i < maxLumps; i++)
		{
			// An array to hold all tiles that has been generated into the lump
			Tile *usedTiles[10];
			int currentIndex = 1;

			// Get a random position in a chunk
			int x = thor::random(0, 7);
			int y = thor::random(0, 7);

			// If its not collidable, we quit this lump
			if (!mTiles[x][y]->getMaterial()->isCollidable()) break;

			// Get a random material
			int randomIndex = thor::random(0, static_cast<int>(lumpables.size() - 1));
			Material* material = lumpables[randomIndex];
			mTiles[x][y]->setMaterial(material);
			usedTiles[0] = mTiles[x][y];

			// Randomize how big the lump is going to be
			int lumpSize = thor::random(material->getMinLumpSize(), material->getMaxLumpSize() - 1);

			for (int j = 0; j < lumpSize - 1; j++)
			{
				iter++;
				// Get random relative tile
				Tile* randomUsedTile = usedTiles[thor::random(0, currentIndex - 1)];
				int randomDirection = thor::random(0, 3);

				sf::Vector2f relative;
				switch (randomDirection)
				{
				case 0: // North
					relative = sf::Vector2f(0, -1);
					break;
				case 1: // East
					relative = sf::Vector2f(1, 0);
					break;
				case 2: // South
					relative = sf::Vector2f(0, 1);
					break;
				case 3: // West
					relative = sf::Vector2f(-1, 0);
					break;
				default:
					break;
				}


				if (randomUsedTile->isPositionInMyChunk(randomUsedTile->getPosition() + relative))
				{
					Tile* relativeTile = randomUsedTile->getRelative(sf::Vector2i(relative));
					if (relativeTile->getMaterial()->isCollidable())
					{
						relativeTile->setMaterial(material);
						usedTiles[currentIndex] = relativeTile;
						currentIndex++;
					}
				}
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

Tile * Chunk::getTile(int x, int y)
{
	return mTiles[x][y];
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

Chunk *Chunk::getRelative(sf::Vector2i pRelativePosition)
{
	sf::Vector2i newChunkPosition = mPosition + pRelativePosition;
	return mWorld->getChunkByChunkPosition(newChunkPosition);
}

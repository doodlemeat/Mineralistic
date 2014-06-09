#include "Chunk.h"
#include "SFML\Graphics.hpp"
#include "World.h"
#include <iostream>
#include "Tile.h"
#include "noiseutils.h"
#include "noise/noise.h"
#include "Logger.h"
#include "TileStop.h"

Chunk::Chunk(World *pWorld)
{
	mWorld = pWorld;

	//
	sprite = new sf::Sprite();
	sprite->setTexture(mWorld->getTileset());
}

Chunk::~Chunk()
{
}

bool Chunk::isPosition(sf::Vector2i pPosition)
{
	return mPosition == pPosition;
}

void Chunk::setPosition(sf::Vector2i pPosition)
{
	mPosition = pPosition;
	sf::Vector2f spritePosition(pPosition.x * mWorld->getChunkSize().x, pPosition.y * mWorld->getChunkSize().y);
	sprite->setPosition(sf::Vector2f(spritePosition));
}

sf::Sprite * Chunk::getSprite()
{
	return sprite;
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
	for (int x = 0; x < mTiles.size(); x++)
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
	for (int x = 0; x < mTiles.size(); x++)
	{
		for (int y = 0; y < mTiles[x].size(); y++)
		{
			// Sometimes libnoise can return a value over 1.0, better be sure to cap the top and bottom..
			float heightValue = pHeightMap->GetValue(x, y);
			if (heightValue > 1.f) heightValue = 1.f;
			if (heightValue < -1.f) heightValue = -1.f;

			// Instantiate a new Tile object with the noise value, this doesn't do anything yet..
			mTiles[x][y] = new Tile(this, pHeightMap->GetValue(x, y));

			// Get a pointer to the current tile's quad
			sf::Vertex *quad = &mVertices[(y + x * pHeightMap->GetWidth()) * 4];
			
			quad[0].position = sf::Vector2f(offsetPositon.x + x * tileSize.x, offsetPositon.y + y * tileSize.y);
			quad[1].position = sf::Vector2f(offsetPositon.x + (x + 1) * tileSize.x, offsetPositon.y + y * tileSize.y);
			quad[2].position = sf::Vector2f(offsetPositon.x + (x + 1) * tileSize.x, offsetPositon.y + (y + 1) * tileSize.y);
			quad[3].position = sf::Vector2f(offsetPositon.x + x * tileSize.x, offsetPositon.y + (y + 1) * tileSize.y);

			// find out which type of tile to render, atm only air or stone
			TileStop *tilestop = mWorld->getTileStopAt(heightValue);
			sf::Vector2i texturePos = tilestop->getTexturePosition();

			// define its 4 texture coordinates
			quad[0].texCoords = sf::Vector2f(texturePos.x, texturePos.y);
			quad[1].texCoords = sf::Vector2f(texturePos.x + 64, texturePos.y);
			quad[2].texCoords = sf::Vector2f(texturePos.x + 64, texturePos.y + 64);
			quad[3].texCoords = sf::Vector2f(texturePos.x, texturePos.y + 64);
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

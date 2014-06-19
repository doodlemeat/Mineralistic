#include "Tile.h"
#include "Material.h"
#include "Chunk.h"
#include "World.h"
#include "Box2D/Box2D.h"
#include "SFML/Graphics/Vertex.hpp"
#include "Logger.h"
#include <iostream>
#include "Math.h"

Tile::Tile(Chunk *pChunk, sf::Vector2f pPosition, Material *pMaterial, b2Body *pBody, sf::Vertex *pQuad)
{
	mChunk = pChunk;
	mMaterial = pMaterial;
	mBody = pBody;
	mPosition = pPosition;
	mQuad = pQuad;
	mTorch = nullptr;
}


Tile::~Tile()
{
}

sf::Vector2f Tile::getPosition()
{
	return mPosition;
}

Material *Tile::getMaterial()
{
	return mMaterial;
}

b2Body *Tile::getBody()
{
	return mBody;
}

void Tile::setBodyNull()
{
	mBody = nullptr;
}

void Tile::setMaterial(Material *pMaterial)
{
	Material *oldMaterial = mChunk->getWorld()->getMaterial(mMaterial->getName());
	mMaterial = pMaterial;
	
	// Set new textureRect
	sf::FloatRect textureRect = static_cast<sf::FloatRect>(mMaterial->getTextureRect());
	
	mQuad[0].texCoords = sf::Vector2f(textureRect.left, textureRect.top) + sf::Vector2f(0.5f, 0.5f);
	mQuad[1].texCoords = sf::Vector2f(textureRect.left + textureRect.width, textureRect.top) + sf::Vector2f(-0.5f, 0.5f);
	mQuad[2].texCoords = sf::Vector2f(textureRect.left + textureRect.width, textureRect.top + textureRect.height) + sf::Vector2f(-0.5f, -0.5f);
	mQuad[3].texCoords = sf::Vector2f(textureRect.left, textureRect.top + textureRect.height) + sf::Vector2f(0.5f, -0.5f);
	
	if (mMaterial->isCollidable())
	{
		if (oldMaterial->isCollidable())
		{
			mChunk->getB2World()->DestroyBody(mBody);
			mBody = nullptr;
		}
		sf::Vector2f vertices[4];
		vertices[0] = mQuad[0].position;
		vertices[1] = mQuad[1].position;
		vertices[2] = mQuad[2].position;
		vertices[3] = mQuad[3].position;
		mBody = mChunk->getWorld()->createChain(vertices, 4);
	}
	else
	{
		if (oldMaterial->isCollidable())
		{
			mChunk->getB2World()->DestroyBody(mBody);
			mBody = nullptr;
		}
	}
}

void Tile::breakNaturally()
{
	setMaterial(mChunk->getWorld()->getMaterial("Air"));
}

Tile *Tile::getRelative(sf::Vector2i pRelativePosition)
{
	sf::Vector2f newPosition;
	newPosition.x = mPosition.x + pRelativePosition.x;
	newPosition.y = mPosition.y + pRelativePosition.y;
	Tile *newTile = mChunk->getWorld()->getTileByWorldPosition(newPosition);
	return newTile;
}

void Tile::updateLight(float pLightLevel)
{
	mLightLevel = pLightLevel;
	float opacity = Math::relativeFromInterval(100, 255, 0, 7, mLightLevel);
	mQuad[0].color = sf::Color(255, 255, 255, opacity);
	mQuad[1].color = sf::Color(255, 255, 255, opacity);
	mQuad[2].color = sf::Color(255, 255, 255, opacity);
	mQuad[3].color = sf::Color(255, 255, 255, opacity);
}

int Tile::getLightLevel()
{
	return mLightLevel;
}

void Tile::setTorch(Torch *pTorch)
{
	mTorch = pTorch;
}

Torch *Tile::getTorch()
{
	return mTorch;
}
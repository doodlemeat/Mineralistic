#include "Tile.h"
#include "Material.h"
#include "Chunk.h"
#include "World.h"
#include "Box2D/Box2D.h"
#include "SFML/Graphics/Vertex.hpp"

Tile::Tile(Chunk *pChunk, sf::Vector2i pPosition, Material *pMaterial, b2Body *pBody, sf::Vertex *pQuad)
{
	mChunk = pChunk;
	mMaterial = pMaterial;
	mBody = pBody;
	mPosition = pPosition;
	mQuad = pQuad;
}


Tile::~Tile()
{
}

sf::Vector2i Tile::getPosition()
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
	Material *oldMaterial = mMaterial;
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
		sf::Vector2f position = static_cast<sf::Vector2f>(mChunk->getPosition() + mPosition);
		mBody = mChunk->getWorld()->createBody(position);
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

#include "Tile.h"
#include "Material.h"
#include "Chunk.h"
#include "World.h"
#include "Box2D/Box2D.h"
#include "SFML/Graphics/Vertex.hpp"
#include "Logger.h"
#include <iostream>
#include "Math.h"
#include "Thor/Math/Distributions.hpp"
#include "Thor/Animation/FadeAnimation.hpp"
#include "Thor/Particles/Affectors.hpp"
#include "BreakAffector.h"

Tile::Tile(Chunk *pChunk, sf::Vector2f pPosition, Material *pMaterial, b2Body *pBody, sf::Vertex *pQuad, sf::Vertex *pLightQuad)
{
	mChunk = pChunk;
	mMaterial = pMaterial;
	mBody = pBody;
	mPosition = pPosition;
	mQuad = pQuad;
	mLightQuad = pLightQuad;
	mTorch = nullptr;
	mIntensity = 0;
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
		}
	}
}

void Tile::breakNaturally()
{
	if (!mMaterial->isCollidable()) return;
	 
	thor::UniversalEmitter emitter = mChunk->getWorld()->getBlockParticleEmitter();
	emitter.setEmissionRate(60);
	emitter.setParticleLifetime(sf::seconds(1.f)); 
	emitter.setParticleRotation(thor::Distributions::uniform(0.f, 360.f));
	emitter.setParticleRotationSpeed(thor::Distributions::uniform(6, 15));
	emitter.setParticlePosition(thor::Distributions::circle(WorldHelper::toSFMLPositionFromWorldPosition(mPosition, true), 10));
	emitter.setParticleVelocity(thor::Distributions::deflect(sf::Vector2f(0, -96), 20));
	emitter.setParticleTextureIndex(mMaterial->getParticleRectIndex());

	thor::ParticleSystem *pSystem = mChunk->getWorld()->getBlockParticleSystem();
	pSystem->addEmitter(emitter, sf::seconds(0.1f));
	setMaterial(mChunk->getWorld()->getMaterial("Air"));
}

Tile *Tile::getRelative(sf::Vector2i pRelativePosition)
{
	return getRelative(pRelativePosition.x, pRelativePosition.y);
}

Tile *Tile::getRelative(int x, int y)
{
	sf::Vector2f newPosition;
	newPosition.x = mPosition.x + x;
	newPosition.y = mPosition.y + y;
	Tile *newTile = mChunk->getWorld()->getTileByWorldPosition(newPosition);
	return newTile;
}

bool Tile::isPositionInMyChunk(sf::Vector2f pPosition)
{
	Chunk* chunk = mChunk;
	sf::Vector2f upper_left_corner = chunk->getTile(sf::Vector2i(0, 0))->getPosition();
	sf::Vector2f lower_right_corner = chunk->getTile(sf::Vector2i(7, 7))->getPosition();
	if (pPosition.x >= upper_left_corner.x && pPosition.x <= lower_right_corner.x &&
		pPosition.y >= upper_left_corner.y && pPosition.y <= lower_right_corner.y)
	{
		return true;
	}
	return false;
}

Chunk * Tile::getChunk()
{
	return mChunk;
}

void Tile::setLightIntensity(float pIntensity)
{
	mIntensity = pIntensity;

	int alpha = (int)Math::relativeFromInterval(0.f, 155.f, 0.f, 7.f, 7.f - mIntensity);
	sf::Color baseLightColor = sf::Color::Black;
	baseLightColor.a = alpha;
	mLightQuad[0].color = baseLightColor;
	mLightQuad[1].color = baseLightColor;
	mLightQuad[2].color = baseLightColor;
	mLightQuad[3].color = baseLightColor;
}

float Tile::getIntensity()
{
	return mIntensity;
}

void Tile::setTorch(Torch *pTorch)
{
	mTorch = pTorch;
}

Torch *Tile::getTorch()
{
	return mTorch;
}

sf::Vertex* Tile::getQuad() const
{
	return mQuad;
}

sf::Vertex* Tile::getLightQuad() const
{
	return mLightQuad;
}

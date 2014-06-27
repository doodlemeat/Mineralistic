#pragma once
#include "Thor\Particles\Particle.hpp"
#include "World.h"
#include "Tile.h"

class World;

class BreakAffector
{
public:

	BreakAffector(World* pWorld);

	void operator() (thor::Particle& particle, sf::Time dt)
	{
		Tile *tile = mWorld->getTileByWorldPosition(WorldHelper::toWorldPositionFromSFMLPosition(particle.position));
		int alpha = tile->getQuad()[0].color.a;
		particle.color.r = alpha;
		particle.color.g = alpha;
		particle.color.b = alpha;
	}

private:
	World *mWorld;
};
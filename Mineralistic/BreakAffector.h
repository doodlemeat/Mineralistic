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
	}

private:
	World *mWorld;
};
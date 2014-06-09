#pragma once

#include <SFML/System/Vector2.hpp>
#include <Box2D/Common/b2Math.h>

namespace PhysicsScale
{
	float gameToPhys(float value);
	float physToGame(float value);

	b2Vec2 gameToPhys(const sf::Vector2f &vec);
	sf::Vector2f physToGame(const b2Vec2 &vec);

	float gameToPhysAngle(float angle);
	float physToGameAngle(float angle);
}
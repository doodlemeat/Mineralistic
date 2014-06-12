#pragma once

#include <Box2D\Common\b2Math.h>
#include <SFML\System\Vector2.hpp>

namespace PhysicsHelper
{
	b2Vec2 gameToPhysicsUnits(sf::Vector2f p_unit);
	b2Vec2 gameToPhysicsUnits(sf::Vector2i p_unit);
	float gameToPhysicsUnits(float p_unit);
	sf::Vector2f physicsToGameUnits(float p_x, float p_y);
	sf::Vector2f physicsToGameUnits(b2Vec2 p_position);
	float physicsToGameUnits(float32 value);
}
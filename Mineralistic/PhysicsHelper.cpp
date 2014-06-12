#include "PhysicsHelper.h"

namespace PhysicsHelper
{
	b2Vec2 gameToPhysicsUnits(sf::Vector2f p_unit)
	{
		return b2Vec2(p_unit.x / 32.f, p_unit.y / 32.f);
	}
	b2Vec2 gameToPhysicsUnits(sf::Vector2i p_unit)
	{
		return b2Vec2(static_cast<float>(p_unit.x) / 32.f, static_cast<float>(p_unit.y) / 32.f);
	}
	float gameToPhysicsUnits(float p_unit)
	{
		return p_unit / 32.f;
	}

	sf::Vector2f physicsToGameUnits(float p_x, float p_y)
	{
		return sf::Vector2f(p_x * 32.f, p_y * 32.f);
	}
	sf::Vector2f physicsToGameUnits(b2Vec2 p_position)
	{
		return sf::Vector2f(p_position.x * 32.f, p_position.y * 32.f);
	}

	float physicsToGameUnits(float32 value)
	{
		return value * 32.f;
	}
}
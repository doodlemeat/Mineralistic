#include "PhysicsScale.h"
#include "Angle.h"

namespace PhysicsScale
{
	static const float PHYS_SCALE = 100.f;
	static const float PHYS_SCALE_INV = 1.f / PHYS_SCALE;

	float gameToPhys(float value)
	{
		return value * PHYS_SCALE_INV;
	}
	float physToGame(float value)
	{
		return value * PHYS_SCALE;
	}

	b2Vec2 gameToPhys(const sf::Vector2f &vec)
	{
		return b2Vec2(gameToPhys(vec.x), gameToPhys(vec.y));
	}
	sf::Vector2f physToGame(const b2Vec2 &vec)
	{
		return sf::Vector2f(physToGame(vec.x), physToGame(vec.y));
	}

	float gameToPhysAngle(float angle)
	{
		return -angle * Angle::DEG2RAD;
	}
	float physToGameAngle(float angle)
	{
		return -angle * Angle::RAD2DEG;
	}
}

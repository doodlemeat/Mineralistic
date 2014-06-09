#pragma once

#include "Math.h"
#include <SFML/System/Vector2.hpp>

namespace Angle
{
	static const float DEG2RAD = Math::PI / 180.f;
	static const float RAD2DEG = 180.f / Math::PI;

	sf::Vector2f getVector(float angle);

	float getAngle(const sf::Vector2f &from, const sf::Vector2f &to);
}

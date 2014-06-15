#include "Angle.h"
#include <cmath>

namespace Angle
{
	sf::Vector2f getVector(float angle)
	{
		return sf::Vector2f(std::cosf(angle), std::sinf(angle));
	}

	float getAngle(const sf::Vector2f &from, const sf::Vector2f &to)
	{
		float angle = std::atan2(from.y, from.x) - std::atan2f(to.y, to.x);
		if (angle > Math::PI)
			angle -= Math::PI * 2;
		if (angle < -Math::PI)
			angle += Math::PI * 2;
		return -angle;
	}
}

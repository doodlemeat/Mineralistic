#pragma once

#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Shape.hpp>
#include <algorithm>
#include <Box2D\Box2D.h>

namespace Math
{
	const float PI = 3.14159265359f;

	bool pointInCircle(sf::Vector2f point, sf::Vector2f circle_center, float circle_radius);

	float euclideanDistance(sf::Vector2f v1, sf::Vector2f v2);

	float vectorLength(sf::Vector2f v);

	float angleBetween(const sf::Vector2f &v1, const sf::Vector2f &v2);

	float roundByMultiple(float n, float multiple);

	float clamp(float n, float max, float min);

	sf::Vector2f direction(sf::Vector2f v1, sf::Vector2f v2);

	float relativeFromInterval(float sourceMin, float sourceMax, float intervalMin, float intervalMax, float n);

	int manhattanDistance(int x1, int y1, int x2, int y2);

	float RAD2DEG(float rad);
	float DEG2RAD(float deg);
}


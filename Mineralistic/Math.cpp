#include "Math.h"
#include <cmath>

namespace Math
{
	bool pointInCircle(sf::Vector2f point, sf::Vector2f circle_center, float circle_radius)
	{
		return (euclideanDistance(point, circle_center) <= circle_radius);
	}

	float euclideanDistance(sf::Vector2f v1, sf::Vector2f v2)
	{
		return std::sqrt(std::pow(v1.x - v2.x, 2) + std::pow(v1.y - v2.y, 2));
	}

	float vectorLength(sf::Vector2f v)
	{
		return std::sqrt(std::pow(v.x, 2) + std::pow(v.y, 2));
	}

	float angleBetween(const sf::Vector2f &v1, const sf::Vector2f &v2)
	{
		return std::atan2f(v2.y - v1.y, v2.x - v1.x);
	}

	float roundByMultiple(float n, float multiple)
	{
		return multiple * (std::roundf(n / multiple));
	}

	float clamp(float n, float max, float min)
	{
		if (n > max) return max;
		if (n < min) return min;
		return n;
	}

	sf::Vector2f direction(sf::Vector2f v1, sf::Vector2f v2)
	{
		sf::Vector2f delta = v2 - v1;
		float length = vectorLength(delta);
		return sf::Vector2f(delta.x / length, delta.y / length);
	}

	float relativeFromInterval(float sourceMin, float sourceMax, float intervalMin, float intervalMax, float n)
	{
		assert(sourceMin < sourceMax);
		assert(intervalMin < intervalMax);

		if (n < intervalMin) n = intervalMin;
		if (n > intervalMax) n = intervalMax;

		float p = (n - intervalMin) / (intervalMax - intervalMin);
		float sum = p * (sourceMax - sourceMin) + sourceMin;
		return sum;
	}

	float RAD2DEG(float rad)
	{
		return rad * 180 / PI;
	}

	float DEG2RAD(float deg)
	{
		return deg * PI / 180;
	}

	int manhattanDistance(int x1, int y1, int x2, int y2)
	{
		return std::abs(x1 - x2) + std::abs(y1 - y2);
	}

	sf::IntRect scaleRect(const sf::IntRect &pRect, float pFactor)
	{
		sf::IntRect scaledRect;
		scaledRect.left = (int)((float)pRect.left * pFactor);
		scaledRect.top = (int)((float)pRect.top * pFactor);
		scaledRect.width = (int)((float)pRect.width * pFactor);
		scaledRect.height = (int)((float)pRect.height * pFactor);
		return scaledRect;
	}

}




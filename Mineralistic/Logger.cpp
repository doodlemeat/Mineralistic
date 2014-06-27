#include "Logger.h"
#include <iostream>

namespace Logger
{
	void vector2(sf::Vector2i pVector)
	{
		std::cout << "Vector2i: " << "(" << pVector.x << "," << pVector.y << ")" << std::endl;
	}
	void vector2(sf::Vector2f pVector)
	{
		std::cout << "Vector2f: " << "(" << pVector.x << "," << pVector.y << ")" << std::endl;
	}
	void vector2(sf::Vector2u pVector)
	{
		std::cout << "Vector2u: " << "(" << pVector.x << "," << pVector.y << ")" << std::endl;
	}

	void info(std::string pOut)
	{
		std::cout << pOut << std::endl;
	}

	void rect(sf::FloatRect pbounds)
	{
		std::cout << "FloatRect: (" << pbounds.left << "," << pbounds.top << "," << pbounds.width << "," << pbounds.height << ")" << std::endl;
	}

	void rect(sf::IntRect pbounds)
	{
		std::cout << "IntRect: (" << pbounds.left << "," << pbounds.top << "," << pbounds.width << "," << pbounds.height << ")" << std::endl;
	}
}

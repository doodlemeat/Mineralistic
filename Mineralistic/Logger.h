#pragma once

#include "SFML/System/Vector2.hpp"
#include <string>

namespace Logger
{
	void vector2(sf::Vector2i pVector);
	void vector2(sf::Vector2f pVector);
	void vector2(sf::Vector2u pVector);
	void info(std::string pOut);
}

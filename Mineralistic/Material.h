#pragma once

#include "SFML/Graphics/Rect.hpp"
#include <string>

class Material
{
public:
	Material(std::string pName, bool pCollidable, sf::IntRect pTextureRect);
	~Material();

	void setCollidable(bool pValue);
	void setTextureRect(sf::IntRect pTextureRect);

	bool isCollidable();
	std::string getName();
	sf::IntRect getTextureRect();

private:
	sf::IntRect mTextureRect;
	std::string mName;
	
	bool mCollidable;
};
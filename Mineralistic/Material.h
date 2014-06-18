#pragma once

#include "SFML/Graphics/Rect.hpp"
#include <string>

class Material
{
public:
	Material(std::string pName, bool pCollidable, sf::IntRect pTextureRect);
	Material(std::string pName, bool pCollidable, sf::IntRect pTextureRect, int mResistance);
	~Material();

	void setCollidable(bool pValue);
	void setTextureRect(sf::IntRect pTextureRect);
	void setResistance(int pResistance);

	bool isCollidable();
	std::string getName();
	sf::IntRect getTextureRect();
	int getResistance();

private:
	sf::IntRect mTextureRect;
	std::string mName;
	int mResistance;
	
	bool mCollidable;
};
#pragma once

#include <string>
#include "SFML\System\Vector2.hpp"
#include "SFML\Graphics\Rect.hpp"

class TileStop
{
public:
	TileStop(std::string pName);
	~TileStop();

	void setHeightStop(float pHeightStop);
	void setTexturePosition(sf::Vector2i pPosition);

	float getHeightStop();
	sf::Vector2i getTexturePosition();
	std::string getName();

private:
	std::string mName;
	float mHeightStop;
	sf::Vector2i mTexturePosition;
};


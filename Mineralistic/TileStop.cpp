#include "TileStop.h"


TileStop::TileStop(std::string pName)
{
	mName = pName;
}


TileStop::~TileStop()
{
}

void TileStop::setHeightStop(float pHeightStop)
{
	mHeightStop = pHeightStop;
}

void TileStop::setTexturePosition(sf::Vector2i pPosition)
{
	mTexturePosition = pPosition;
}

float TileStop::getHeightStop()
{
	return mHeightStop;
}

sf::Vector2i TileStop::getTexturePosition()
{
	return mTexturePosition;
}

std::string TileStop::getName()
{
	return mName;
}

#include "TileStop.h"
#include "Material.h"

TileStop::TileStop(Material *pMaterial)
{
	mMaterial = pMaterial;
}

TileStop::~TileStop()
{
}

void TileStop::setHeightStop(float pHeightStop)
{
	mHeightStop = pHeightStop;
}

float TileStop::getHeightStop()
{
	return mHeightStop;
}

Material *TileStop::getMaterial()
{
	return mMaterial;
}

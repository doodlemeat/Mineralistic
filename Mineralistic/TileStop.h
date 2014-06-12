#pragma once

#include <string>
#include "SFML\System\Vector2.hpp"
#include "SFML\Graphics\Rect.hpp"

class Material;

class TileStop
{
public:
	TileStop(Material *pMaterial);
	~TileStop();

	void setHeightStop(float pHeightStop);
	float getHeightStop();

	Material *getMaterial();

private:
	Material *mMaterial;
	float mHeightStop;
};


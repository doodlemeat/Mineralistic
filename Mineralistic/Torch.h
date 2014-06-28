#pragma once

#include "GameObject.h"

class Tile;

class Torch : public GameObject
{
public:
	Torch(int pWidth, int pHeight);
	~Torch();

	void update(float dt);
	void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	void processNeighbors(int pX, int pY, float pIntensity);
	void setTile(Tile *pTile);
	void lightsOff();
	void smoothLight(int pX, int pY);
	void collectTiles();

private:
	bool mComplete;
	sf::Color lightColor;
	int mWidth;
	int mHeight;
	Tile *mTile;
	Tile **mTilesInRange;
};
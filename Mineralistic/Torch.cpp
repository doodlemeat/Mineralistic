#include "Torch.h"
#include "SFML\Graphics\RenderTarget.hpp"
#include "Tile.h"
#include "Material.h"
#include <iostream>
#include "Logger.h"


Torch::Torch(int pWidth, int pHeight)
{
	mWidth = pWidth;
	mHeight = pHeight;
}


Torch::~Torch()
{
	delete[] mTilesInRange;
}

void Torch::update(float dt)
{
	if (!mDead)
	{
		processNeighbors(mWidth >> 1, mHeight >> 1, 7.f);
	}
}

void Torch::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(*mSprite);
}

void Torch::smoothLight(int pX, int pY)
{
	if (pX == 0 || pX == (mWidth - 1) || pY == 0 || pY == (mHeight - 1)) return;

	// Tile we are smoothing
	Tile *c = mTilesInRange[pY * mWidth + pX];
	if (c == nullptr) return;

	int x_min_1 = pX - 1;
	int x_plus_1 = pX + 1;
	int y_min_1 = pY - 1;
	int y_plus_1 = pY + 1;

	// Get all nearby tiles in all 8 weather directions
	Tile *NW = mTilesInRange[y_min_1 * mWidth + x_min_1];
	Tile *N = mTilesInRange[y_min_1 * mWidth + pX];
	Tile *NE = mTilesInRange[y_min_1 * mWidth + x_plus_1];
	Tile *E = mTilesInRange[pY * mWidth + x_plus_1];
	Tile *SW = mTilesInRange[y_plus_1 * mWidth + x_min_1];
	Tile *S = mTilesInRange[y_plus_1 * mWidth + pX];
	Tile *SE = mTilesInRange[y_plus_1 * mWidth + x_plus_1];
	Tile *W = mTilesInRange[pY * mWidth + x_min_1];

	/*
		- Loop trough all 4 vertices on the tile in the following order

		0 1
		2 3

		- For each vertex, find the 3 other vertices that lies on the same point, example vertex 1:

		0 1  <--
		2 3

							0 1 0 1
							2 3 2 3
		Original quad   ->	0 1 0 1
						->	2 3 2 3

		Result: 3 2
				1 0
	*/
	for (int i = 0; i < 4; i++)
	{
		int intensitySum = 0;

		// This is really ugly but I can't come up with a better algorithm
		if (i == 0) // upper left
		{
			intensitySum += c->getQuad()[0].color.a; // me
			intensitySum += N->getQuad()[3].color.a;
			intensitySum += NW->getQuad()[2].color.a;
			intensitySum += W->getQuad()[1].color.a;

			int alpha = intensitySum / 4;
			c->getQuad()[0].color.a = alpha;
		} 
		else if (i == 1) // upper right
		{
			intensitySum += c->getQuad()[1].color.a; // me
			intensitySum += N->getQuad()[2].color.a;
			intensitySum += NE->getQuad()[3].color.a;
			intensitySum += E->getQuad()[0].color.a;

			int alpha = intensitySum / 4;
			c->getQuad()[1].color.a = alpha;
		}
		else if (i == 2) // lower left
		{
			intensitySum += c->getQuad()[2].color.a; // me
			intensitySum += E->getQuad()[3].color.a;
			intensitySum += SE->getQuad()[0].color.a;
			intensitySum += S->getQuad()[1].color.a;

			int alpha = intensitySum / 4;
			c->getQuad()[2].color.a = alpha;
		}
		else if (i == 3) // lower right
		{
			intensitySum += c->getQuad()[3].color.a; // me
			intensitySum += S->getQuad()[0].color.a;
			intensitySum += SW->getQuad()[1].color.a;
			intensitySum += W->getQuad()[2].color.a;

			int alpha = intensitySum / 4;
			c->getQuad()[3].color.a = alpha;
		}
	}
}

void Torch::processNeighbors(int pX, int pY, float pIntensity)
{
	Tile* current = mTilesInRange[pY * mWidth + pX];
	current->setLightIntensity(pIntensity);
	float newIntensity = pIntensity - 1.f;
	if (newIntensity <= 0.f)
	{
		return;
	}

	int x_min_1 = pX - 1;
	int x_plus_1 = pX + 1;
	int y_min_1 = pY - 1;
	int y_plus_1 = pY + 1;

	Tile *N = mTilesInRange[y_min_1 * mWidth + pX];  // North
	Tile *E = mTilesInRange[pY * mWidth + x_plus_1]; // East
	Tile *S = mTilesInRange[y_plus_1 * mWidth + pX]; // South
	Tile *W = mTilesInRange[pY * mWidth + x_min_1];  // West
	if (N->getIntensity() <= newIntensity)
	{
			N->setLightIntensity(newIntensity);
			processNeighbors(pX, y_min_1, newIntensity);
	}
	if (E->getIntensity() <= newIntensity)
	{
			E->setLightIntensity(newIntensity);
			processNeighbors(x_plus_1, pY, newIntensity);
	}
	if (S->getIntensity() <= newIntensity)
	{
			S->setLightIntensity(newIntensity);
			processNeighbors(pX, y_plus_1, newIntensity);
	}
	if (W->getIntensity() <= newIntensity)
	{
			W->setLightIntensity(newIntensity);
			processNeighbors(x_min_1, pY, newIntensity);
	}
}

void Torch::setTile(Tile *pTile)
{
	mTile = pTile;

	// Allocate space for tiles
	mTilesInRange = new Tile*[(mWidth * mHeight)];

	// Set default value of nullptr
	for (int i = 0; i < (mWidth * mHeight); i++)
	{
		mTilesInRange[i] = nullptr;
	}

	int width = mWidth;
	if (!(mWidth & 1))
	{
		width--;
	}

	int height = mHeight;
	if (!(mHeight & 1))
	{
		height--;
	}

	// Gather affeced tiles
	int cx = mWidth >> 1;
	int cy = mHeight >> 1;
	int row = 0;
	int column = 0;
	for (int x = -cx; x < cx; x++)
	{
		row = 0;
		for (int y = -cy; y < cy; y++)
		{
			Tile *tile = mTile->getRelative(x, y);
			mTilesInRange[row * mWidth + column] = tile;
			row++;
		}
		column++;
	}
	processNeighbors(mWidth >> 1, mHeight >> 1, 7.f);
}

void Torch::lightsOff()
{
	for (int i = 0; i < (mWidth * mHeight); i++)
	{
		mTilesInRange[i]->setLightIntensity(0);
	}
}

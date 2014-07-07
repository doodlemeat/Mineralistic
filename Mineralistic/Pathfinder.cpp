#include "Pathfinder.h"
#include "Math.h"
#include "BinaryHeap.h"
#include "Node.h"
#include "World.h"
#include "Logger.h"
#include <iostream>
#include "Tile.h"
#include "Material.h"


Pathfinder::Pathfinder(World* pWorld, int pWidth, int pHeight)
{
	mWorldRef = pWorld;
	mWidth = pWidth;
	mHeight = pHeight;
	mNodes = new Node[mWidth * mHeight];
}

Pathfinder::~Pathfinder()
{
	delete[] mNodes;
}


void Pathfinder::computeGrid(const sf::Vector2i &pStart, const sf::Vector2i &pGoal)
{
	mStartWorldPos = pStart;
	mGoalWorldPos = pGoal;
	if (mStartWorldPos.x < 0) mStartWorldPos.x -= 1;
	if (mStartWorldPos.y < 0) mStartWorldPos.y -= 1;
	if (mGoalWorldPos.x < 0) mGoalWorldPos.x -= 1;
	if (mGoalWorldPos.y < 0) mGoalWorldPos.y -= 1;
	sf::Vector2f midPoint = Math::midPoint(mStartWorldPos, mGoalWorldPos);
	mGridArea = Math::getRectByMidPoint(midPoint, mHeight, mWidth);
}

void Pathfinder::search()
{
	init();

	BinaryHeap openHeap;

	mStart->g = 0;
	mStart->h = manhattan(mStart, mGoal);
	mStart->opened = true;

	openHeap.push(mStart);

	while (openHeap.size() > 0)
	{
		// Pop the node with the smallest f value
		Node* currentNode = openHeap.pop();

		// Check if we hit the target
		if (currentNode == mGoal)
		{
			// Compute the path and exit
			generatePath(currentNode);
			return;
		}

		currentNode->closed = true;

		std::vector<Node*> neighbors = getNeighborsAdjacentTo(currentNode);
		for (int i = 0; i < neighbors.size(); i++)
		{
			Node* neighbor = neighbors[i];
			if (neighbor->closed || neighbor->worldRef->getMaterial()->isCollidable())
				continue;

			int gScore = currentNode->g + 1;

			if(!neighbor->opened || gScore < neighbor->g)
			{
				neighbor->g = currentNode->g + 1;
				neighbor->h = manhattan(currentNode, neighbor);
				neighbor->parent = currentNode;
				neighbor->opened = true;
				openHeap.push(neighbor);
			}
		}
	}
}

void Pathfinder::init()
{
	mPath.clear();

	int startX = mGridArea.left;
	int startY = mGridArea.top;
	int capX = startX + mWidth;
	int capY = startY + mHeight;

	int iX = 0;
	int iY = 0;

	for (int x = startX; x < capX; x++)
	{
		iY = 0;
		for (int y = startY; y < capY; y++)
		{
			Node node;
			node.worldRef = mWorldRef->getTileByWorldPosition(sf::Vector2f(x, y));
			node.x = iX;
			node.y = iY;
			mNodes[iY * mWidth + iX] = node;
			sf::Vector2i pos = sf::Vector2i(node.worldRef->getPosition());
			if (mStartWorldPos.x == pos.x && mStartWorldPos.y == pos.y)
			{
				mStart = &mNodes[iY * mWidth + iX];
			}
			else if (mGoalWorldPos.x == pos.x && mGoalWorldPos.y == pos.y)
			{
				mGoal = &mNodes[iY * mWidth + iX];
			}
			iY++;
		}
		iX++;
	}
}

World* Pathfinder::getWorldRef() const
{
	return mWorldRef;
}

int Pathfinder::manhattan(Node* pN1, Node* pN2)
{
	int d1 = std::abs(pN2->x - pN1->x);
	int d2 = std::abs(pN2->y - pN1->y);
	return d1 + d2;
}

void Pathfinder::generatePath(Node* pCurrentNode)
{
	Node* current = pCurrentNode;
	mPath.push_back(current);
	while (current->parent != nullptr)
	{
		mPath.push_back(current);
		current = current->parent;
	}
}

std::vector<Node*> Pathfinder::getNeighborsAdjacentTo(Node* pCurrentNode)
{
	std::vector<Node*> foundNeighbors;
	int x = pCurrentNode->x;
	int y = pCurrentNode->y;

	int x_min = x - 1;
	int x_plus = x + 1;
	int y_min = y - 1;
	int y_plus = y + 1;

	int width = mWidth;

	int north_index = y_min * width + x;
	int east_index = y * width + x_plus;
	int south_index = y_plus * width + x;
	int west_index = y * width + x_min;

	int limit = width * width - 1;

	// North
	if (north_index >= 0 && north_index <= limit)
		foundNeighbors.push_back(&mNodes[north_index]);

	// East
	if (east_index >= 0 && east_index <= limit)
		foundNeighbors.push_back(&mNodes[east_index]);
	
	// South
	if (south_index >= 0 && south_index <= limit)
		foundNeighbors.push_back(&mNodes[south_index]);
	
	// West
	if (west_index >= 0 && west_index <= limit)
		foundNeighbors.push_back(&mNodes[west_index]);
	
	return foundNeighbors;
}

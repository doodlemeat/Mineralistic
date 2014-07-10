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
	mStart = nullptr;
	mGoal = nullptr;
	mNodes = new Node[mWidth * mHeight];
}

Pathfinder::~Pathfinder()
{
	delete[] mNodes;
}


void Pathfinder::computeGrid(sf::Vector2f pStart, sf::Vector2f pGoal)
{
	if (pStart.x <= 0.f) pStart.x = std::floor(pStart.x);
	if (pStart.y <= 0.f) pStart.y = std::floor(pStart.y);

	if (pGoal.x <= 0.f) pGoal.x = std::floor(pGoal.x);
	if (pGoal.y <= 0.f) pGoal.y = std::floor(pGoal.y);

	mStartWorldPos = sf::Vector2i(pStart);
	mGoalWorldPos = sf::Vector2i(pGoal);

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
		
		int x = current->x;
		int y = current->y;
		if (current->parent != nullptr)
		{
			int px = current->parent->x;
			int py = current->parent->y;

			if (px == x && y > py)
			{
				current->jumpable = true;
			}
		}

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

bool Pathfinder::hasChanged(sf::Vector2f pStartPos, sf::Vector2f pGoalPos)
{
	if (mStart == nullptr || mGoal == nullptr) return true;
	if (mStart->worldRef != mWorldRef->getTileByWorldPosition(pStartPos)) 
	{
		return true;
	}
	if (mGoal->worldRef != mWorldRef->getTileByWorldPosition(pGoalPos))
	{
		return true;
	}
	return false;
}

Node* Pathfinder::jump(int pX, int pY, int pPx, int pPy)
{
	/*int dx = pX - pPx;
	int dy = pY - pPy;
	int x = pX;
	int y = pY;

	Node* node = &mNodes[y * mWidth + x];

	if (node->worldRef->getMaterial()->isCollidable)
	{
		return nullptr;
	}

	if (node == mGoal)
	{
		return node;
	}

	// check forced neigbors along the diagonal
	if (dx != 0 && dy != 0)
	{
		if (!getNode(x - dx, y + dy).worldRef->getMaterial()->isCollidable &&
			getNode(x - dx, y).worldRef->getMaterial()->isCollidable() ||
			!getNode(x + dx, y - dy).worldRef->getMaterial()->isCollidable() &&
			getNode(x, y - dy).worldRef->getMaterial()->isCollidable())
		{
			return node;
		}
	}
	else
	{
		if (dx != 0)
		{
			if (!getNode(x + dx, y + 1).worldRef->getMaterial()->isCollidable() &&
				getNode(x, y + 1).worldRef->getMaterial()->isCollidable() ||
				!getNode(x + dx, y - 1).worldRef->getMaterial()->isCollidable() &&
				getNode(x, y - 1).worldRef->getMaterial()->isCollidable())
			{
				return node;
			}
		}
		else
		{
			if (!getNode(x + 1, y).worldRef->getMaterial()->isCollidable() &&
				getNode(x + 1, y + dy).worldRef->getMaterial()->isCollidable() ||
				!getNode(x - 1, y + dy).worldRef->getMaterial()->isCollidable() &&
				getNode(x - 1, y).worldRef->getMaterial()->isCollidable())
			{
				return node;
			}
		}
	}

	if (dx != 0 && dy != 0)
	{
		if (jump(x + dx, y, x, y) != nullptr || jump(x, y + dy, x, y) != nullptr)
		{
			return node;
		}
	}

	if (!getNode(x + dx, y).worldRef->getMaterial()->isCollidable() ||
		!getNode(x, y + dy).worldRef->getMaterial()->isCollidable())
	{
		return jump(x + dx, y + dy, x, y);
	}
	else
	{
		return nullptr;
	}*/
	return nullptr;
}

Node& Pathfinder::getNode(int pX, int pY)
{
	return mNodes[pY * mWidth + pX];
}

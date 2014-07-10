#pragma once

#include <vector>
#include "SFML\System\Vector2.hpp"
#include "SFML\Graphics\Rect.hpp"


class Tile;
struct Node;
class World;

class Pathfinder
{
public:
	Pathfinder(World* pWorld, int pWidth, int pHeight);
	~Pathfinder();

	void computeGrid(sf::Vector2f pStart, sf::Vector2f pGoal);
	void search();
	void init();
	void clearPath() { mPath.clear(); }
	void generatePath(Node* pCurrentNode);
	Node* jump(int pX, int pY, int pPx, int pPy);
	Node& getNode(int pX, int pY);

	int manhattan(Node* pN1, Node* pN2);

	std::vector<Node*>& getPath() { return mPath; }
	World* getWorldRef() const;
	Node*& getNodes() { return mNodes; }
	std::vector<Node*> getNeighborsAdjacentTo(Node* pCurrentNode);
	bool hasChanged(sf::Vector2f pGoalPos, sf::Vector2f pStartPos);
private:
	sf::Vector2i mStartWorldPos;
	sf::Vector2i mGoalWorldPos;

	int mWidth;
	int mHeight;

	Node* mStart;
	Node* mGoal;
	World* mWorldRef;
	Node* mNodes;

	sf::IntRect mGridArea;

	std::vector<Node*> mPath;
};


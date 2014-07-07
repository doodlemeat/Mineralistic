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

	void computeGrid(const sf::Vector2i &pStart, const sf::Vector2i &pGoal);
	void search();
	void init();
	void clearPath() { mPath.clear(); }
	void generatePath(Node* pCurrentNode);

	int manhattan(Node* pN1, Node* pN2);

	std::vector<Node*>& getPath() { return mPath; }
	World* getWorldRef() const;
	Node*& getNodes() { return mNodes; }
	std::vector<Node*> getNeighborsAdjacentTo(Node* pCurrentNode);
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


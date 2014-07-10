#pragma once
#include "SFML\System\Vector2.hpp"

class Tile;

struct Node
{
	Node();
	~Node();

	int f() { return g + h; }

	Node* parent;
	Tile* worldRef;
	int g;
	int h;
	int x;
	int y;
	bool closed;
	bool opened;
	bool jumpable;
};


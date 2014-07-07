#include "Node.h"


Node::Node()
{
	parent = nullptr;
	worldRef = nullptr;
	closed = false;
	opened = false;
	g = 0;
	h = 0;
	x = 0;
	y = 0;
}

Node::~Node()
{
}
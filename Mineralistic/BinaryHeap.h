#pragma once

#include <vector>

struct Node;

class BinaryHeap
{
public:
	BinaryHeap();
	~BinaryHeap();

	void push(Node* pElement);
	Node* pop();
	void sink(int pIndex);
	void rise(int pIndex);
	int size() { return mElements.size(); }

private:
	std::vector<Node*> mElements;
};
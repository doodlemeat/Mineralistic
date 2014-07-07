#include "BinaryHeap.h"
#include "Node.h"


BinaryHeap::BinaryHeap()
{
}


BinaryHeap::~BinaryHeap()
{
}

void BinaryHeap::push(Node* pElement)
{
	mElements.push_back(pElement);
	sink(mElements.size() - 1);
}

void BinaryHeap::sink(int pIndex)
{
	// Fetch element that will try to sink
	Node* element = mElements[pIndex];

	// When index at 0, the element can not sink further
	while (pIndex > 0)
	{
		// Compute the parents index and fetch it
		int parentIndex = ((pIndex + 1) >> 1) - 1; // std::floor((pIndex + 1) / 2);
		Node* parent = mElements[parentIndex];

		// Swap the elements if the parent is greater
		if (parent->f() > element->f())
		{
			mElements[parentIndex] = element;
			mElements[pIndex] = parent;

			// Update 'pIndex' to continue at the new position
			pIndex = parentIndex;
		}
		// We found a parent that is less
		else
		{
			break;
		}
	}
}

Node* BinaryHeap::pop()
{
	// Store the first element so that we can return it later
	Node* result = mElements[0];

	// Get the last element in the vector
	Node* end = mElements.back();
	mElements.pop_back();

	if (mElements.size() > 0)
	{
		mElements[0] = end;
		rise(0);
	}

	return result;
}

void BinaryHeap::rise(int pIndex)
{
	int length = mElements.size();
	Node* element = mElements[pIndex];
	int elementScore = element->f();

	while (true)
	{
		// Compute indices of child elements
		int child2N = (pIndex + 1) << 1;
		int child1N = child2N - 1;

		int swap = -1;
		
		// If the first child exists
		if (child1N < length)
		{
			// Lookup child and compute score
			Node* child1 = mElements[child1N];
			int child1Score = child1->f();

			// If the score is less than our element, we need to swap them
			if (child1Score < elementScore)
			{
				swap = child1N;
			}
		}
		
		// Do the same for this child
		if (child2N < length)
		{
			Node* child2 = mElements[child2N];
			int child2Score = child2->f();

			if (child2Score < elementScore)
			{
				swap = child2N;
			}
		}

		// If the element needs to be moved, swap it and continue
		if (swap != -1)
		{
			mElements[pIndex] = mElements[swap];
			mElements[swap] = element;
			pIndex = swap;
		}
		else
		{
			break;
		}
	}
}

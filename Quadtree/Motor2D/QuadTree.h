#ifndef _QUAD_TREE_
#define _QUAD_TREE_

#include <list>
#include <vector>
#include "Collision.h"

#define MAX_OBJECTS 2 //Number of obects in a node before split
#define NODE_COUNT 4 //4 nodes. NW, NE, SW, SE. 

using namespace std;

class QuadTree
{
public:
	QuadTree(SDL_Rect treeArea);
	~QuadTree();
	void ClearTree();
	void SplitNode();
	int GetNodeIndex(Collider* col);
	void Insert(Collider* col);
	void Retrieve(list<Collider*> &possibleColliders, Collider* col);
	bool Contains(Collider* col);
	bool Intersects(Collider* col);
	void GetParentNodes(vector<QuadTree*>& node);

public:
	list<Collider*> colllidersList;
	SDL_Rect nodeRect;
	QuadTree* node[4];
	QuadTree* parent;
	int nodeIndex;
};

#endif // !_QUAD_TREE_


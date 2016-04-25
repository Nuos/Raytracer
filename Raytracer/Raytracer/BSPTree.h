#ifndef _BSPTREE_H
#define _BSPTREE_H

#include "SPTree.h"

struct EventPoint
{
	float position;
	int id;
};

class BSPTree : public SPTree
{
public:
	BSPTree( List<Mesh*> *&meshList, BoundingBox boundingBox, int maxDepth = 10, int threshold = 50, bool useSAH = false, float traversalCost = 0.001 );
	~BSPTree();

	bool raycast( Ray &ray, Vector3 &intersection, Vector3 &normal, Material *&material );
	void subdivide( SPTreeNode *&node, int currentDepth, int splitAxis );
	void subdivideSAH( SPTreeNode *&node, int currentDepth, int splitAxis );

	void initializeExtremePoints();

	bool raycastNode( SPTreeNode *&node, Ray &ray, Vector3 &intersection, Vector3 &normal, Material *&material, float tMin, float tMax, int splitAxis );

private:
	bool useSAH;
	float traversalCost;
};

#endif
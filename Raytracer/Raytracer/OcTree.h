#ifndef _OCTREE_H
#define _OCTREE_H

#include "SPTree.h"

class OcTree : public SPTree
{
public:
	OcTree( List<Mesh*> *&meshList, BoundingBox boundingBox, int maxDepth = 10, int threshold = 50 );
	~OcTree();

	bool raycast( Ray &ray, Vector3 &intersection, Vector3 &normal, Material *&material );
	void subdivide( SPTreeNode *&node, int currentDepth );

	bool raycastNode( SPTreeNode *&node, Ray &ray, Vector3 &intersection, Vector3 &normal, Material *&material );
};

#endif
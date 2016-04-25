#ifndef _SPTREENODE_H
#define _SPTREENODE_H

#include "List.h"
#include "Face.h"
#include "Ray.h"
#include "Vector3.h"
#include "Material.h"

class SPTreeNode
{
public:
	SPTreeNode( int childCount, int capacity );
	~SPTreeNode();

	bool raycast( Ray &ray, Vector3 &intersection, Vector3 &normal, Material *&material );

	inline int getChildCount(){ return childCount; }

	BoundingBox boundingBox;
	List<Face*> *faceList;
	SPTreeNode **childNodes;

	float split;
	
private:
	int childCount;
};

#endif
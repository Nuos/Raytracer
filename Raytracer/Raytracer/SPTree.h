#ifndef _SPTREE_H
#define _SPTREE_H

#include "SPTreeNode.h"
#include "List.h"
#include "Mesh.h"
#include "BoundingBox.h"
#include "Material.h"

#include "Screen.h"

class SPTree
{
public:
	SPTree( List<Mesh*> *&meshList, BoundingBox boundingBox, int childNodeCount = 2, int maxDepth = 10, int threshold = 50 );
	~SPTree();

	// void build( int maxDepth, int threshold );
	virtual bool raycast( Ray &ray, Vector3 &intersection, Vector3 &normal, Material *&material ) = 0;

	int getAverageTriangleCountPerLeaf();

	void draw();
	void drawGUI();

	void setVisualisationEnabled( bool value ){ showTree = value; }
	void stopAnimation(){ animDelta = 0; }
	void resumeAnimation(){ animDelta = 0.025; }
	void resetAnimation(){ drawingDepth = 0; }


	int getMaxDepth(){ return maxDepth; }
	int getThreshold(){ return threshold; }

	int intersectionTestCount;

	static int rayID;

protected:
	void initializeRoot();
	void drawNode( SPTreeNode *&node, int depth );

	int maxDepth, threshold, childNodeCount;

	BoundingBox boundingBox;

	Face **faces;
	List<Mesh*> *meshList;
	SPTreeNode *root;

	int totalFaceCount;

	char* name;

	float drawingDepth;
	bool showTree;
	float animDelta;
};

#endif

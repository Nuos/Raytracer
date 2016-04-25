#include "Octree.h"
#include <iostream>
#include <time.h>
#include "Raytracer.h"

struct ClosestIntersection
{
	float t;
	int index;
};

void inline sort(ClosestIntersection* c, int length)
{
	for (int i=1; i<length; ++i) // use pre-increment to avoid unneccessary temorary object
    {
        ClosestIntersection key= c[i];
        int j = i-1;
		while((j >= 0) && (c[j].t > key.t))
        {
                c[j+1] = c[j];
                j -= 1;
        }
        c[j+1]=key;
    }
}

OcTree::OcTree( List<Mesh*> *&_meshList, BoundingBox _boundingBox, int _maxDepth, int _threshold ) :
	SPTree( _meshList, _boundingBox, 8, _maxDepth, _threshold )
{
	name = "OcTree";

	clock_t init, final;
	init = clock();

	subdivide( root, 0 );

	final=clock()-init;
	std::cout << name << " is built in " << final / ((float)CLOCKS_PER_SEC) << " seconds" << std::endl;
}

bool OcTree::raycast( Ray &ray, Vector3 &intersection, Vector3 &normal, Material *&material )
{
	float t;
	SPTree::rayID++;

	if ( ray.intersects( root->boundingBox, t ) )
		return raycastNode( root, ray, intersection, normal, material );
	else 
		return false;
}

void OcTree::subdivide( SPTreeNode *&node, int currentDepth )
{
	if ( currentDepth >= maxDepth )
		return;

	if ( node->faceList->size() < threshold )
		return;

	int faceCount = node->faceList->size();

	int nodeCapacity = faceCount / 4 + 4;

	Vector3 boundingBoxSize = ( node->boundingBox.max - node->boundingBox.min ) * 0.5;

	node->childNodes[0] = new SPTreeNode( childNodeCount, nodeCapacity );
	node->childNodes[0]->boundingBox = node->boundingBox;
	node->childNodes[0]->boundingBox.max = node->childNodes[0]->boundingBox.min + boundingBoxSize;

	node->childNodes[1] = new SPTreeNode( childNodeCount, nodeCapacity );
	node->childNodes[1]->boundingBox = node->boundingBox;
	node->childNodes[1]->boundingBox += Vector3(boundingBoxSize.x, 0, 0);
	node->childNodes[1]->boundingBox.max = node->childNodes[1]->boundingBox.min + boundingBoxSize;

	node->childNodes[2] = new SPTreeNode( childNodeCount, nodeCapacity );
	node->childNodes[2]->boundingBox = node->boundingBox;
	node->childNodes[2]->boundingBox += Vector3(0, boundingBoxSize.y, 0);
	node->childNodes[2]->boundingBox.max = node->childNodes[2]->boundingBox.min + boundingBoxSize;

	node->childNodes[3] = new SPTreeNode( childNodeCount, nodeCapacity );
	node->childNodes[3]->boundingBox = node->boundingBox;
	node->childNodes[3]->boundingBox += Vector3(0, 0, boundingBoxSize.z);
	node->childNodes[3]->boundingBox.max = node->childNodes[3]->boundingBox.min + boundingBoxSize;

	node->childNodes[4] = new SPTreeNode( childNodeCount, nodeCapacity );
	node->childNodes[4]->boundingBox = node->boundingBox;
	node->childNodes[4]->boundingBox += Vector3(boundingBoxSize.x, boundingBoxSize.y, 0);
	node->childNodes[4]->boundingBox.max = node->childNodes[4]->boundingBox.min + boundingBoxSize;

	node->childNodes[5] = new SPTreeNode( childNodeCount, nodeCapacity );
	node->childNodes[5]->boundingBox = node->boundingBox;
	node->childNodes[5]->boundingBox += Vector3(boundingBoxSize.x, 0, boundingBoxSize.z);
	node->childNodes[5]->boundingBox.max = node->childNodes[5]->boundingBox.min + boundingBoxSize;

	node->childNodes[6] = new SPTreeNode( childNodeCount, nodeCapacity );
	node->childNodes[6]->boundingBox = node->boundingBox;
	node->childNodes[6]->boundingBox += Vector3(0, boundingBoxSize.y, boundingBoxSize.z);
	node->childNodes[6]->boundingBox.max = node->childNodes[6]->boundingBox.min + boundingBoxSize;

	node->childNodes[7] = new SPTreeNode( childNodeCount, nodeCapacity );
	node->childNodes[7]->boundingBox = node->boundingBox;
	node->childNodes[7]->boundingBox += Vector3(boundingBoxSize.x, boundingBoxSize.y, boundingBoxSize.z);
	node->childNodes[7]->boundingBox.max = node->childNodes[7]->boundingBox.min + boundingBoxSize;
	
	for ( int i = 0; i < faceCount; i++ )
	{
		Face *face = node->faceList->get(i);

		for ( int j = 0; j < childNodeCount; j++ )
		{
			if ( face->intersects( node->childNodes[j]->boundingBox ) )
			{
				node->childNodes[j]->faceList->add( face );
			}
		}
	}

	delete node->faceList;
	node->faceList = NULL;

	for ( int i = 0; i < childNodeCount; i++ )
	{
		subdivide( node->childNodes[i], currentDepth + 1 );
	}
}

bool OcTree::raycastNode( SPTreeNode *&node, Ray &ray, Vector3 &intersection, Vector3 &normal, Material *&material )
{
	if ( node->childNodes[0] == NULL )
	{
		Raytracer::intersectionTestCount += node->faceList->size();
		return node->raycast( ray, intersection, normal, material );
	}
	else
	{
		ClosestIntersection closestIntersections[8];
		int count = 0;

		for ( int i = 0; i < 8; i++ )
		{
			float t = 0;

			if ( ray.intersects(node->childNodes[i]->boundingBox, t) )
			{
				closestIntersections[count].index = i;
				closestIntersections[count++].t = t;

				if (count > 3) // ??? at most 4 intersection, but check
					break;
			}
		}

		sort(closestIntersections, count);

		for ( int i = 0; i < count; i++ )
		{
			if ( raycastNode( node->childNodes[closestIntersections[i].index], ray, intersection, normal, material) )
				return true;
		}

		return false;
	}
}
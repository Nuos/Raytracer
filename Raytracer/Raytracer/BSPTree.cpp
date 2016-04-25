#include "BSPTree.h"
#include <iostream>
#include <ctime>
#include "Raytracer.h"

#include <algorithm>

BSPTree::BSPTree( List<Mesh*> *&_meshList, BoundingBox _boundingBox, int _maxDepth, int _threshold, bool _useSAH, float _traversalCost ) : 
	SPTree( _meshList, _boundingBox, 2, _maxDepth, _threshold )
{
	useSAH = _useSAH;
	traversalCost = _traversalCost;

	if( useSAH )
		name = "kD-Tree";
	else
		name = "BSP tree";

	clock_t init, final;
	init = clock();

	if( useSAH )
	{
		initializeExtremePoints();

		subdivideSAH( root, 0, 0 );
	}
	else
		subdivide( root, 0, 0 );

	final=clock()-init;
	std::cout << name << " is built in " << final / ((float)CLOCKS_PER_SEC) << " seconds" << std::endl;
}

BSPTree::~BSPTree()
{
}

bool BSPTree::raycast( Ray &ray, Vector3 &intersection, Vector3 &normal, Material *&material )
{
	rayID++;

	float tMin, tMax;

	if ( ray.intersects(root->boundingBox, tMin, tMax ) )
	{
		return raycastNode( root, ray, intersection, normal, material, tMin, tMax, 0 );
	}
	else 
		return false;	
}

void BSPTree::subdivide( SPTreeNode *&node, int currentDepth, int splitAxis )
{
	if ( currentDepth >= maxDepth )
		return;

	if ( node->faceList->size() < threshold )
		return;

	int faceCount = node->faceList->size();

	node->childNodes[0] = new SPTreeNode( 2, faceCount / 2  + 1); // Crazy bug was there=)
	node->childNodes[1] = new SPTreeNode( 2, faceCount / 2  + 1);

	node->childNodes[0]->boundingBox = node->boundingBox;
	node->childNodes[1]->boundingBox = node->boundingBox;

	if( splitAxis == 0 )
	{
		float sizeX = ( node->boundingBox.max.x - node->boundingBox.min.x ) / 2;

		node->childNodes[0]->boundingBox.max.x = node->childNodes[0]->boundingBox.max.x - sizeX;
		node->split = node->childNodes[0]->boundingBox.max.x;
		node->childNodes[1]->boundingBox.min.x = node->childNodes[1]->boundingBox.min.x + sizeX;
	}
	else if( splitAxis == 1)
	{
		float sizeY = ( node->boundingBox.max.y - node->boundingBox.min.y ) / 2;

		node->childNodes[0]->boundingBox.max.y = node->childNodes[0]->boundingBox.max.y - sizeY;
		node->split = node->childNodes[0]->boundingBox.max.y;
		node->childNodes[1]->boundingBox.min.y = node->childNodes[1]->boundingBox.min.y + sizeY;
	}
	else if( splitAxis == 2)
	{
		float sizeZ = ( node->boundingBox.max.z - node->boundingBox.min.z ) / 2;

		node->childNodes[0]->boundingBox.max.z = node->childNodes[0]->boundingBox.max.z - sizeZ;
		node->split = node->childNodes[0]->boundingBox.max.z;
		node->childNodes[1]->boundingBox.min.z = node->childNodes[1]->boundingBox.min.z + sizeZ;
	}
	
	for ( int i = 0; i < faceCount; i++ )
	{
		Face *face = node->faceList->get(i);

		if ( face->intersects( node->childNodes[0]->boundingBox ) )
			node->childNodes[0]->faceList->add( face );

		if ( face->intersects( node->childNodes[1]->boundingBox ) )
			node->childNodes[1]->faceList->add( face );
	}

	delete node->faceList;
	node->faceList = NULL;

	if (splitAxis == 2)
		splitAxis = 0;
	else
		splitAxis++;

	subdivide( node->childNodes[0], currentDepth + 1, splitAxis );
	subdivide( node->childNodes[1], currentDepth + 1, splitAxis );
}

inline bool EPlessThan ( EventPoint &elem1, EventPoint &elem2 )
{
	return elem1.position < elem2.position;
}

void BSPTree::subdivideSAH( SPTreeNode *&node, int currentDepth, int splitAxis )
{
	//cout << currentDepth << endl;
	//if( currentDepth >= maxDepth )
	//	return;

	//if( node->faceList->size() < threshold )
	//	return;

	int faceCount = node->faceList->size();

	int eventPointCount = faceCount * 2;
	EventPoint* eventPoints = new EventPoint[eventPointCount];


	if( splitAxis == 0 )
	{
		for( int i = 0; i < faceCount; i += 1 )
		{
			Face* face = node->faceList->get( i  );

			eventPoints[i].position = face->vertices[ face->extremes[1] ].x;
			eventPoints[i].id = i ;

			eventPoints[i+faceCount].position = face->vertices[ face->extremes[0] ].x;
			eventPoints[i+faceCount].id = i ;
		}
	}
	else if( splitAxis == 1 )
	{
		for( int i = 0; i < faceCount; i += 1 )
		{
			Face* face = node->faceList->get( i  );

			eventPoints[i].position = face->vertices[ face->extremes[3] ].y;
			eventPoints[i].id = i ;

			eventPoints[i+faceCount].position = face->vertices[ face->extremes[2] ].y;
			eventPoints[i+faceCount].id = i ;
		}
	}
	else
	{
		for( int i = 0; i < faceCount; i += 1 )
		{
			Face* face = node->faceList->get( i  );

			eventPoints[i].position = face->vertices[ face->extremes[5] ].z;
			eventPoints[i].id = i ;

			eventPoints[i+faceCount].position = face->vertices[ face->extremes[4] ].z;
			eventPoints[i+faceCount].id = i ;
		}
	} 

	int bestSplitIndex = 0;

	sort( eventPoints, eventPoints + eventPointCount, EPlessThan ); // stable sort

	char *eventPointStatus = new char[faceCount];

	for( int i = 0; i < faceCount; i++ )
		eventPointStatus[i] = 0;

	int leftCount = 0;
	int rightCount = faceCount;

	float leftPosition, rightPosition, baseArea, perimeter;

	float baseCost = 2 * ( node->boundingBox.max.y - node->boundingBox.min.y ) * ( node->boundingBox.max.z - node->boundingBox.min.z ) +
					 2 * ( node->boundingBox.max.x - node->boundingBox.min.x ) * ( node->boundingBox.max.z - node->boundingBox.min.z ) +
					 2 * ( node->boundingBox.max.y - node->boundingBox.min.y ) * ( node->boundingBox.max.x - node->boundingBox.min.x ) ;

	baseCost *= faceCount;

	float minCost = baseCost;

	if( splitAxis == 0 )
	{
		leftPosition = node->boundingBox.min.x;
		rightPosition = node->boundingBox.max.x;
		baseArea = 2 * ( node->boundingBox.max.y - node->boundingBox.min.y ) * ( node->boundingBox.max.z - node->boundingBox.min.z );
		perimeter = 2 * ( ( node->boundingBox.max.y - node->boundingBox.min.y ) + ( node->boundingBox.max.z - node->boundingBox.min.z ) ) ;
	}
	else if( splitAxis == 1 )
	{
		leftPosition = node->boundingBox.min.y;
		rightPosition = node->boundingBox.max.y;
		baseArea = 2 * ( node->boundingBox.max.x - node->boundingBox.min.x ) * ( node->boundingBox.max.z - node->boundingBox.min.z );
		perimeter = 2 * ( ( node->boundingBox.max.x - node->boundingBox.min.x ) + ( node->boundingBox.max.z - node->boundingBox.min.z ) ) ;
	}
	else
	{
		leftPosition = node->boundingBox.min.z;
		rightPosition = node->boundingBox.max.z;
		baseArea = 2 * ( node->boundingBox.max.x - node->boundingBox.min.x ) * ( node->boundingBox.max.y - node->boundingBox.min.y );
		perimeter = 2 * ( ( node->boundingBox.max.x - node->boundingBox.min.x ) + ( node->boundingBox.max.y - node->boundingBox.min.y ) ) ;
	}

	int straddle = 0;

	bool found = false;

	for( int i = 0; i < eventPointCount; i++ )
	{
		eventPointStatus[ eventPoints[i].id ]++;
		bool isLeft = false;
		if( eventPointStatus[ eventPoints[i].id ] == 1 )
		{
			straddle++;
			isLeft = true;
		}
		else if( eventPointStatus[ eventPoints[i].id ] == 2 )
		{
			straddle--;
			rightCount--;
		}

		leftCount += isLeft;

		float cost =  traversalCost * baseCost + 
				    ( baseArea + perimeter * abs( eventPoints[i].position - leftPosition ) ) * leftCount +
				    ( baseArea + perimeter * abs( rightPosition - eventPoints[i].position ) ) * rightCount;

		//if( leftCount == 0 )
		//	cost *= 0.8;

		//if( rightCount == 0 )
		//	cost *= 0.8;

		//if( (float)straddle / faceCount < 0.01 )
		//	cost *= 0.9;

		

		if( ( cost < minCost ) && (eventPoints[i].position > leftPosition && eventPoints[i].position < rightPosition) )
		{
			minCost = cost;
			bestSplitIndex = i;
			found = true;
		}
	}

	if ( !found )
	{
		delete[] eventPoints;
		delete[] eventPointStatus;

		return;
	}

	node->childNodes[0] = new SPTreeNode( 2, faceCount / 2  + 1);
	node->childNodes[1] = new SPTreeNode( 2, faceCount / 2  + 1);

	node->childNodes[0]->boundingBox = node->boundingBox;
	node->childNodes[1]->boundingBox = node->boundingBox;

	if( eventPoints[bestSplitIndex].position < leftPosition || eventPoints[bestSplitIndex].position > rightPosition )
		std::cout << "Error ";

	for( int i = 0; i < faceCount; i++ )
		eventPointStatus[i] = 0;

	for( int i = 0; i < bestSplitIndex; i++ )
		eventPointStatus[ eventPoints[i].id ]++;

	if( eventPointStatus[ eventPoints[bestSplitIndex].id ] == 1 )
		eventPointStatus[ eventPoints[bestSplitIndex].id ]++;

	for( int i = 0; i < faceCount; i++ )
	{
		Face *face = node->faceList->get( i );

		if( eventPointStatus[i] == 2 )
			node->childNodes[0]->faceList->add( face );
		else if( eventPointStatus[i] == 0 )
			node->childNodes[1]->faceList->add( face );
		else
		{
			node->childNodes[0]->faceList->add( face );
			node->childNodes[1]->faceList->add( face );
		}
	}

	node->split = eventPoints[bestSplitIndex].position;

	if( splitAxis == 0 )
	{
		node->childNodes[0]->boundingBox.max.x = eventPoints[bestSplitIndex].position;
		node->childNodes[1]->boundingBox.min.x = eventPoints[bestSplitIndex].position;
	}
	else if( splitAxis == 1 )
	{
		node->childNodes[0]->boundingBox.max.y = eventPoints[bestSplitIndex].position;
		node->childNodes[1]->boundingBox.min.y = eventPoints[bestSplitIndex].position;
	}
	else
	{
		node->childNodes[0]->boundingBox.max.z = eventPoints[bestSplitIndex].position;
		node->childNodes[1]->boundingBox.min.z = eventPoints[bestSplitIndex].position;
	}

	delete[] eventPoints;
	delete[] eventPointStatus;

	delete node->faceList;
	node->faceList = NULL;

	subdivideSAH( node->childNodes[0], currentDepth + 1, (splitAxis+1)%3 );
	subdivideSAH( node->childNodes[1], currentDepth + 1, (splitAxis+1)%3 );
}

bool BSPTree::raycastNode( SPTreeNode *&node, Ray &ray, Vector3 &intersection, Vector3 &normal, Material *&material, float tMin, float tMax, int splitAxis )
{
	if ( node->childNodes[0] == NULL )
	{
		Raytracer::intersectionTestCount += node->faceList->size();
		return node->raycast( ray, intersection, normal, material );
	}
	else
	{
		SPTreeNode *near, *far;

		if( splitAxis == 0 )
		{
			float tSplit = (node->split - ray.origin.x) / ray.direction.x;

			near = ray.origin.x < node->split ? node->childNodes[0] : node->childNodes[1];
			far = ray.origin.x < node->split ? node->childNodes[1] : node->childNodes[0];

			if (tSplit > tMax)
				return raycastNode( near, ray, intersection, normal, material, tMin, tMax, splitAxis + 1 );
			else if (tSplit < tMin)
			{
				if( tSplit > 0 )
					return raycastNode( far, ray, intersection, normal, material, tMin, tMax, splitAxis + 1  );//case B
				else if(tSplit < 0)
					return raycastNode( near, ray, intersection, normal, material, tMin, tMax, splitAxis + 1  );//case C
				else
				{
					if(ray.direction.x < 0)
						return raycastNode( far, ray, intersection, normal, material, tMin, tMax, splitAxis + 1  );//case D
					else
						return raycastNode( near, ray, intersection, normal, material, tMin, tMax, splitAxis + 1  );//case E
				}
			}
			else
			{
				if(tSplit > 0)
				{
					if( raycastNode( near, ray, intersection, normal, material, tMin, tSplit, splitAxis + 1 ) )
						return true;

					return raycastNode( far, ray, intersection, normal, material, tSplit, tMax, splitAxis + 1  );
				}
				else{
					return raycastNode( near, ray, intersection, normal, material, tSplit, tMax, splitAxis + 1  );//case G
				}
			}
		}
		else if( splitAxis == 1 )
		{
			float tSplit = (node->split - ray.origin.y) / ray.direction.y;

			near = ray.origin.y < node->split ? node->childNodes[0] : node->childNodes[1];
			far = ray.origin.y < node->split ? node->childNodes[1] : node->childNodes[0];

			if (tSplit > tMax)
				return raycastNode( near, ray, intersection, normal, material, tMin, tMax, splitAxis + 1 );
			else if (tSplit < tMin)
			{
				if( tSplit > 0 )
					return raycastNode( far, ray, intersection, normal, material, tMin, tMax, splitAxis + 1  );//case B
				else if(tSplit < 0)
					return raycastNode( near, ray, intersection, normal, material, tMin, tMax, splitAxis + 1  );//case C
				else
				{
					if(ray.direction.y < 0)
						return raycastNode( far, ray, intersection, normal, material, tMin, tMax, splitAxis + 1  );//case D
					else
						return raycastNode( near, ray, intersection, normal, material, tMin, tMax, splitAxis + 1  );//case E
				}
			}
			else
			{
				if(tSplit > 0)
				{
					if( raycastNode( near, ray, intersection, normal, material, tMin, tSplit, splitAxis + 1 ) )
						return true;

					return raycastNode( far, ray, intersection, normal, material, tSplit, tMax, splitAxis + 1  );
				}
				else{
					return raycastNode( near, ray, intersection, normal, material, tSplit, tMax, splitAxis + 1  );//case G
				}
			}
		}
		else
		{
			float tSplit = (node->split - ray.origin.z) / ray.direction.z;

			near = ray.origin.z < node->split ? node->childNodes[0] : node->childNodes[1];
			far = ray.origin.z < node->split ? node->childNodes[1] : node->childNodes[0];

			if (tSplit > tMax)
				return raycastNode( near, ray, intersection, normal, material, tMin, tMax, 0 );
			else if (tSplit < tMin)
			{
				if( tSplit > 0 )
					return raycastNode( far, ray, intersection, normal, material, tMin, tMax, 0  );//case B
				else if(tSplit < 0)
					return raycastNode( near, ray, intersection, normal, material, tMin, tMax, 0  );//case C
				else
				{
					if(ray.direction.z < 0)
						return raycastNode( far, ray, intersection, normal, material, tMin, tMax, 0  );//case D
					else
						return raycastNode( near, ray, intersection, normal, material, tMin, tMax, 0  );//case E
				}
			}
			else
			{
				if(tSplit > 0)
				{
					if( raycastNode( near, ray, intersection, normal, material, tMin, tSplit, 0 ) )
						return true;

					return raycastNode( far, ray, intersection, normal, material, tSplit, tMax, 0  );
				}
				else{
					return raycastNode( near, ray, intersection, normal, material, tSplit, tMax, 0  );//case G
				}
			}
		}

		return false;
	}
}

void BSPTree::initializeExtremePoints()
{
	int faceCount = root->faceList->size();

	Face *face;

	for( int i = 0; i < faceCount; i++ )
	{
		face = root->faceList->get( i );

		short smallestIndex = 0;
		float smallest = face->vertices[0].x;
		short biggestIndex = 0;
		float biggest = face->vertices[0].x;

		for( int j = 1; j < 3; j++ )
		{
			if( face->vertices[j].x < smallest )
			{
				smallestIndex = j;
				smallest = face->vertices[j].x;
			}

			if( face->vertices[j].x > biggest )
			{
				biggestIndex = j;
				biggest = face->vertices[j].x;
			}
		}

		face->extremes[0] = smallestIndex;
		face->extremes[1] = biggestIndex;

		smallestIndex = 0;
		smallest = face->vertices[0].y;
		biggestIndex = 0;
		biggest = face->vertices[0].y;;

		for( int j = 1; j < 3; j++ )
		{
			if( face->vertices[j].y < smallest )
			{
				smallestIndex = j;
				smallest = face->vertices[j].y;
			}

			if( face->vertices[j].y > biggest )
			{
				biggestIndex = j;
				biggest = face->vertices[j].y;
			}
		}

		face->extremes[2] = smallestIndex;
		face->extremes[3] = biggestIndex;

		smallestIndex = 0;
		smallest = face->vertices[0].z;
		biggestIndex = 0;
		biggest = face->vertices[0].z;

		for( int j = 1; j < 3; j++ )
		{
			if( face->vertices[j].z < smallest )
			{
				smallestIndex = j;
				smallest = face->vertices[j].z;
			}

			if( face->vertices[j].z > biggest )
			{
				biggestIndex = j;
				biggest = face->vertices[j].z;
			}
		}

		face->extremes[4] = smallestIndex;
		face->extremes[5] = biggestIndex;
	}
}

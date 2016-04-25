#ifndef _OCTREE_NODE_H
#define _OCTREE_NODE_H

#include "BoundingBox.h"
#include "List.h"
#include "Face.h"
#include "Mesh.h"
#include "Ray.h"
//#include "Octree.h"
#include <stdlib.h>


class OctreeNode
{
public:

	OctreeNode( int capacity = 256 )
	{
		for (int i = 0; i < 8; i++)
			children[i] = NULL;

		faceList = new List<Face*>( capacity );

	}

	~OctreeNode()
	{
		if (children[0] != NULL)
		{
			for (int i = 0; i < 8; i++)
			{
				delete children[i];
				children[i] = NULL;
			}
		}

		if ( faceList != NULL)
		{
			delete faceList;
			faceList = NULL;
		}
	}

	bool raycast(Ray &ray, Vector3 &intersection, Vector3 &normal, Material *&material)
	{
		float minT = 10000000000;
		bool found = false;
		float a1, b1, c1;
		int index = -1;
		Vector3 *vertices;
		//Vector3 *baryCoords;
		Face *face;

		for ( int i = 0; i < faceList->size(); i++ )
		{
			face = faceList->get(i);
			float t;

			//if ( face->mailbox == rayID )
			//{
			//	//std::cout << "hit";
			//	t = face->t;
			//	vertices = face->vertices;
			//	baryCoords = face->baryCoords;
			//}
			//else
			//{
				vertices = face->vertices;
				//baryCoords = face->baryCoords;
				t = -Vector3::Dot( (ray.origin - vertices[0]),  face->planeNormal ) / Vector3::Dot(ray.direction, face->planeNormal );
			//	face->mailbox = rayID;
			//	face->t = t;
			//}

			if( (t > 0) && (t < minT) )
			{
				intersection = ray.origin + t * ray.direction;

				if ( intersection.x <= boundingBox.max.x && intersection.x >= boundingBox.min.x &&
					 intersection.y <= boundingBox.max.y && intersection.y >= boundingBox.min.y &&
					 intersection.z <= boundingBox.max.z && intersection.z >= boundingBox.min.z )
				{
					float a = ( (vertices[1].y - vertices[2].y) * (intersection.x - vertices[2].x)  + (vertices[2].x - vertices[1].x) * (intersection.y - vertices[2].y) ) /
								( (vertices[1].y - vertices[2].y) * (vertices[0].x - vertices[2].x)  + (vertices[2].x - vertices[1].x) * (vertices[0].y - vertices[2].y) );

					if ( a <= 1 && a >= 0 )
					{
						float b = ( (vertices[2].y - vertices[0].y) * (intersection.x - vertices[2].x)  + (vertices[0].x - vertices[2].x) * (intersection.y - vertices[2].y) ) /
									( (vertices[2].y - vertices[0].y) * (vertices[1].x - vertices[2].x)  + (vertices[0].x - vertices[2].x) * (vertices[1].y - vertices[2].y) );

						if ( b <= 1 && b >= 0 )
						{
							float c = 1 - a - b;

							if( c <= 1 && c >= 0)
							{
								found = true;
								minT = t;
								index = i;
								a1 = a;
								b1 = b;
								c1 = c;
							}
						}
					}
				}	
			}
		}

		if( found )
		{
			Vector3 *normals = faceList->get(index)->normals;
			material = faceList->get(index)->material;

			normal =  a1 * normals[0] + b1 * normals[1] + c1 * normals[2];
			normal.normalize();
			intersection = ray.origin + minT * ray.direction;// + normal * 0.0001;

			return true;
		}	
		
		return false;
	}

	BoundingBox boundingBox;

	OctreeNode *children[8];
	List<Face*> *faceList;

	
	static int rayID;
};

#endif
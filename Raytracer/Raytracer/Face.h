#ifndef _FACE_H
#define _FACE_H

#include "BoundingBox.h"
#include "Material.h"

class Face
{
public:
	Face();
	~Face();

	bool intersects(BoundingBox &boundingBox);

	//Vector3 baryCoords[3];
	Vector3 vertices[3];
	Vector3 normals[3];
	Material* material;
	//short int projectionAxis;
	Vector3 planeNormal;
	short extremes[6]; // event points of the triangle
	
	//int a[4];

	int mailbox;
	float t;
};

#endif
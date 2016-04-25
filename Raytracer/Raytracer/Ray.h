#ifndef _RAY_H
#define _RAY_H

#include "BoundingBox.h"

class Ray
{
public:
	bool intersects( BoundingBox &boundingBox, float &t );
	bool intersects( BoundingBox &boundingBox, float &tMin, float &tMax );

	Vector3 origin;
	Vector3 direction;
};

#endif
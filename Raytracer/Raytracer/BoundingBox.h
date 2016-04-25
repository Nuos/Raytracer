#ifndef _BOUNDINGBOX_H
#define _BOUNDINGBOX_H

#include "Vector3.h"
//#include "Face.h"
#include "Utils.h"

class BoundingBox
{
public:
	BoundingBox();

	void draw( Vector3 &color = Vector3(0.35f,0.6f,0.4f), bool lineWidth = 1, float alpha = 1 );

	BoundingBox operator+=(BoundingBox &other);
	BoundingBox operator+=(Vector3 &other);

	Vector3 min, max;
	float colorScale;
};

#endif
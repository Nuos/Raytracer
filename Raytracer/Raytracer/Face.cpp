#include "Face.h"
#include "Ray.h"

Face::Face()
{
	//mailbox = -1;
}

Face::~Face()
{
}

bool Face::intersects( BoundingBox &boundingBox )
{
	if (vertices[0].x < boundingBox.min.x && 
		vertices[1].x < boundingBox.min.x &&
		vertices[2].x < boundingBox.min.x )
		return false;

	if (vertices[0].x > boundingBox.max.x && 
		vertices[1].x > boundingBox.max.x &&
		vertices[2].x > boundingBox.max.x )
		return false;

	if (vertices[0].y < boundingBox.min.y && 
		vertices[1].y < boundingBox.min.y &&
		vertices[2].y < boundingBox.min.y )
		return false;

	if (vertices[0].y > boundingBox.max.y && 
		vertices[1].y > boundingBox.max.y &&
		vertices[2].y > boundingBox.max.y )
		return false;

	if (vertices[0].z < boundingBox.min.z && 
		vertices[1].z < boundingBox.min.z &&
		vertices[2].z < boundingBox.min.z )
		return false;

	if (vertices[0].z > boundingBox.max.z && 
		vertices[1].z > boundingBox.max.z &&
		vertices[2].z > boundingBox.max.z )
		return false;

	//Ray r;
	//float t;

	//r.origin = vertices[0];
	//r.direction = vertices[0]-vertices[1];
	//
	//if ( r.intersects(boundingBox, t) )
	//	if ( t <= 1 && t <= 0 )
	//		return true;

	//r.origin = vertices[1];
	//r.direction = vertices[1]-vertices[0];
	//
	//if ( r.intersects(boundingBox, t) )
	//	if ( t <= 1 && t <= 0 )
	//		return true;

	//r.origin = vertices[2];
	//r.direction = vertices[2]-vertices[0];
	//
	//if ( r.intersects(boundingBox, t) )
	//	if ( t <= 1 && t <= 0 )
	//		return true;


	return true;

	//if (vertices[0].x < boundingBox.max.x && 
	//	vertices[0].x > boundingBox.min.x &&
	//	vertices[0].y < boundingBox.max.y && 
	//	vertices[0].y > boundingBox.min.y &&
	//	vertices[0].z < boundingBox.max.z && 
	//	vertices[0].z > boundingBox.min.z ) 
	//	return true;

	//if (vertices[1].x < boundingBox.max.x && 
	//	vertices[1].x > boundingBox.min.x &&
	//	vertices[1].y < boundingBox.max.y && 
	//	vertices[1].y > boundingBox.min.y &&
	//	vertices[1].z < boundingBox.max.z && 
	//	vertices[1].z > boundingBox.min.z ) 
	//	return true;

	//if (vertices[2].x < boundingBox.max.x && 
	//	vertices[2].x > boundingBox.min.x &&
	//	vertices[2].y < boundingBox.max.y && 
	//	vertices[2].y > boundingBox.min.y &&
	//	vertices[2].z < boundingBox.max.z && 
	//	vertices[2].z > boundingBox.min.z )
	//	return true;

	//	return false;
}
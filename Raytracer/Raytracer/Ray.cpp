#include "Ray.h"

bool Ray::intersects( BoundingBox &boundingBox, float &t )
{
	t = 1000000000;
	float temp;

	temp = (boundingBox.min.x - origin.x ) / direction.x;

	if ( temp < t && temp >= 0 )
	{
		float y = origin.y + temp * direction.y;
		float z = origin.z + temp * direction.z;

		if ( y <= boundingBox.max.y && y >= boundingBox.min.y &&
			 z <= boundingBox.max.z && z >= boundingBox.min.z )
			 t = temp;
	}
		
	temp = (boundingBox.max.x - origin.x ) / direction.x; 

	if ( temp < t && temp >= 0 )
	{
		float y = origin.y + temp * direction.y;
		float z = origin.z + temp * direction.z;

		if ( y <= boundingBox.max.y && y >= boundingBox.min.y &&
			 z <= boundingBox.max.z && z >= boundingBox.min.z )
			 t = temp;
	}

	temp = (boundingBox.min.y - origin.y ) / direction.y; 

	if ( temp < t && temp >= 0 )
	{
		float x = origin.x + temp * direction.x;
		float z = origin.z + temp * direction.z;

		if ( x <= boundingBox.max.x && x >= boundingBox.min.x &&
			 z <= boundingBox.max.z && z >= boundingBox.min.z )
			 t = temp;
	}

	temp = (boundingBox.max.y - origin.y ) / direction.y; 

	if ( temp < t && temp >= 0 )
	{
		float x = origin.x + temp * direction.x;
		float z = origin.z + temp * direction.z;

		if ( x <= boundingBox.max.x && x >= boundingBox.min.x &&
			 z <= boundingBox.max.z && z >= boundingBox.min.z )
			 t = temp;
	}

	temp = (boundingBox.min.z - origin.z ) / direction.z; 

	if ( temp < t && temp >= 0 )
	{
		float x = origin.x + temp * direction.x;
		float y = origin.y + temp * direction.y;

		if ( x <= boundingBox.max.x && x >= boundingBox.min.x &&
			 y <= boundingBox.max.y && y >= boundingBox.min.y )
			 t = temp;
	}

	temp = (boundingBox.max.z - origin.z ) / direction.z; 

	if ( temp < t && temp >= 0 )
	{
		float x = origin.x + temp * direction.x;
		float y = origin.y + temp * direction.y;

		if ( x <= boundingBox.max.x && x >= boundingBox.min.x &&
			 y <= boundingBox.max.y && y >= boundingBox.min.y )
			 t = temp;
	}

	if( t < 10000000 )
	{
		return true;
	}
	else
		return false;
}

bool Ray::intersects( BoundingBox &boundingBox, float &minT, float &maxT )
{
	minT =  1000000000;
	maxT = -1000000000;

	float temp;

	temp = (boundingBox.min.x - origin.x ) / direction.x;

	float y = origin.y + temp * direction.y;
	float z = origin.z + temp * direction.z;

	if ( y <= boundingBox.max.y && y >= boundingBox.min.y &&
			z <= boundingBox.max.z && z >= boundingBox.min.z )
	{
		if( temp < minT )
			minT = temp;

		if( temp > maxT )
			maxT = temp;
	}
		
	temp = (boundingBox.max.x - origin.x ) / direction.x; 

	y = origin.y + temp * direction.y;
	z = origin.z + temp * direction.z;

	if ( y <= boundingBox.max.y && y >= boundingBox.min.y &&
			z <= boundingBox.max.z && z >= boundingBox.min.z )
	{
		if( temp < minT )
			minT = temp;

		if( temp > maxT )
			maxT = temp;
	}

	temp = (boundingBox.min.y - origin.y ) / direction.y; 

	float x = origin.x + temp * direction.x;
	z = origin.z + temp * direction.z;

	if ( x <= boundingBox.max.x && x >= boundingBox.min.x &&
			z <= boundingBox.max.z && z >= boundingBox.min.z )
	{
		if( temp < minT )
			minT = temp;

		if( temp > maxT )
			maxT = temp;
	}

	temp = (boundingBox.max.y - origin.y ) / direction.y; 

	x = origin.x + temp * direction.x;
	z = origin.z + temp * direction.z;

	if ( x <= boundingBox.max.x && x >= boundingBox.min.x &&
			z <= boundingBox.max.z && z >= boundingBox.min.z )
	{
		if( temp < minT )
			minT = temp;

		if( temp > maxT )
			maxT = temp;
	}

	temp = (boundingBox.min.z - origin.z ) / direction.z; 

	x = origin.x + temp * direction.x;
	y = origin.y + temp * direction.y;

	if ( x <= boundingBox.max.x && x >= boundingBox.min.x &&
			y <= boundingBox.max.y && y >= boundingBox.min.y )
	{
		if( temp < minT )
			minT = temp;

		if( temp > maxT )
			maxT = temp;
	}

	temp = (boundingBox.max.z - origin.z ) / direction.z; 

	x = origin.x + temp * direction.x;
	y = origin.y + temp * direction.y;

	if ( x <= boundingBox.max.x && x >= boundingBox.min.x &&
			y <= boundingBox.max.y && y >= boundingBox.min.y )
	{
		if( temp < minT )
			minT = temp;

		if( temp > maxT )
			maxT = temp;
	}

	if( minT < 1000000 && maxT > -1000000 )
	{
		return true;
	}
	else
		return false;
}
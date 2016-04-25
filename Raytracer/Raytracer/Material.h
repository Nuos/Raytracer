#ifndef _MATERIAL_H
#define _MATERIAL_H

#include "Vector3.h"

class Material
{
public:
	Material( Vector3 _color = Vector3( 1, 1, 1), float _reflectivity = 0, float _transparency = 0 )
	{
		color = _color;
		reflectivity = _reflectivity;
		transparency = _transparency;
	}

	Vector3 color;
	float reflectivity, transparency;
};

#endif
#ifndef _RAYTRACER_H
#define _RAYTRACER_H

#include <iostream>
#include <glut.h>
#include "Vector3.h"
#include "Ray.h"
#include "Material.h"

class Scene;

class Raytracer
{
public:
	Raytracer(Scene* scene);
	~Raytracer();

	void raytrace();
	void drawFrameBuffer();
	void initializeScreen();
	void initializeLights();
	void calculatePerspectiveBounds();
	void setResolution(int width, int height);

	bool useFalseColor;

	static int intersectionTestCount;

private:

	struct UnitLight
	{
		Vector3 position;
		Vector3 color;
		float specularIntensity;
	};

	Vector3 raycastToScene( Ray &ray, int currentDepth );
	Vector3 raycastToLights( Vector3 &intersection, Vector3 &normal, Material *&material, Vector3 &uv );

	Scene* scene;
	int width, height;
	Vector3 topLeft, topRight, bottomLeft, camPos, verticalIncrement, horizontalIncrement;
	UnitLight* unitLights;
	int unitLightCount;

	Vector3 **frameBuffer;
};

#endif
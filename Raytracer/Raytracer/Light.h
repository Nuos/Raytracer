#include "SceneObject.h"

#ifndef _LIGHT_H
#define _LIGHT_H

class Light : public SceneObject 
{
public:
	Light();
	Light(Vector3 position, Vector3 color);

	void draw();
	void drawGUI();
	void update();
	void setLightType( int type ); // 0: point, 1: area
	void initializeLight( int index );
	void calculateBoundingBox();

	static void resetLights()
	{
		for(int i = 0; i < 8; i++)
			glDisable(GL_LIGHT0 + i);
	}

	Vector3 color;
	float specularIntensity;
	int sampleSize;
	float size;
	bool isArea;
};

#endif
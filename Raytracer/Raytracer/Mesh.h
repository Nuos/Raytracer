#ifndef _MESH_H
#define _MESH_H

#include <fstream>
#include <iostream>
#include <glut.h>

#include "BoundingBox.h"
#include "SceneObject.h"
#include "Vector3.h"
#include "Material.h"

using namespace std;

class Mesh : public SceneObject
{
public:
	Mesh();
	Mesh(char* fileName, int normalFactor = 1 );
	
	~Mesh();

	void draw();
	void drawGUI();
	void update();
	void centerVertices();
	void calculateBoundingBox();

	int faceCount;
	void loadFromFile(char* fileName);

//private:
	Vector3 *vertices;
	Vector3 *normals;
	Material *material;

	int normalFactor;
};

#endif
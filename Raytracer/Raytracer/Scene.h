#ifndef _SCENE_H
#define _SCENE_H

#include "BoundingBox.h"
#include "List.h"
#include "GUI.h"
#include "Input.h"
#include "Light.h"
#include "Camera.h"
#include "SceneObject.h"
#include "Mesh.h"
#include <glut.h>
#include <time.h>
#include "Raytracer.h"

#include "OcTree.h"
#include "BSPTree.h"


class Scene
{
public:
	Scene();
	void draw( int width, int height );
	void drawGUI( int width, int height );
	void update();

	SceneObject* pickObject( int x, int y );

	void recalculateBoundingBox();

	void addLight( Light* light );
	void addMesh( Mesh* mesh );
	void deleteActiveSceneObject();

	List<Mesh*>* meshList;
	List<Light*>* lightList;

	Camera *camera;
	Mesh *mesh, *mesh2;
	Light* light;

	SceneObject* activeSceneObject;
	
	BoundingBox boundingBox;

	bool isWireframe;
	bool isBoundingBoxEnabled;
	bool hideMeshes;

	SPTree* sceneTree;
	Raytracer* raytracer;

	bool isRendering;
	float depth;

	int treeDepth;
	int threshold;
};

#endif
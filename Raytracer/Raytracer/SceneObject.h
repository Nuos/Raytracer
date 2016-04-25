#ifndef _SCENEOBJECT_H
#define _SCENEOBJECT_H

#include "Vector3.h"
#include "Transform.h"
#include "BoundingBox.h"
#include "GUI.h"

enum GizmoMode{
	NONE, TX, TY, TZ, RX, RY, RZ
};

class SceneObject
{
public:
	SceneObject();
	SceneObject(Vector3 position);

	void setPosition(float x, float y, float z);
	void setPosition( Vector3 position );
	void setRotation(float x, float y, float z);

	virtual void draw() = 0;
	virtual void drawGUI() = 0;
	void updateTranslationGizmo();
	virtual void update() = 0;

	virtual void calculateBoundingBox() = 0;

	Transform transform;
	BoundingBox boundingBox;

	GizmoMode gizmoMode;

	// GUI stuff
	Vector3 tStartX, tEndX;
	Vector3 tStartY, tEndY;
	Vector3 tStartZ, tEndZ;
	int xTangents[20][2];
	int yTangents[20][2];
	int zTangents[20][2];
};

#endif
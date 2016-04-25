#ifndef _TRANSFORM_H
#define _TRANSFORM_H

#include "Vector3.h"
#include "Matrix.h"

class Transform
{
public:
	Transform();

	void setPosition(Vector3 position);
	void setPosition(float x, float y, float z);
	void translate(Vector3 delta);
	void translate(float x, float y, float z);
	void setRotation(Vector3 rotation);
	void setRotation(float x, float y, float z);
	void rotate(float angle, int axis);
	void lookAt(Vector3 position);

	Matrix getRotationMatrix();

	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

	Vector3 up;
	Vector3 left;
	Vector3 forward;

	Matrix transformationMatrix;

private:
	void calculateTransformationMatrix();
};

#endif
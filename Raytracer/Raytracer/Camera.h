#ifndef _CAMERA_H
#define _CAMERA_H

#include "Input.h"
#include "SceneObject.h"

class Camera : public SceneObject
{
public:
	Camera();
	Camera(Vector3 _position, Vector3 _lookAt, Vector3 _up = Vector3(0,1,0), float _fov = 45, float _near = 0.01, float _far = 100);
	void draw();
	void drawGUI(){};
	void update();
	void initializeCamera( int width, int height );

	void calculateBoundingBox(){}

	void Camera::orbitHorizontal(float angle);
	void Camera::orbitVertical(float angle);

	void Camera::panHorizontal(float angle);

	Vector3 lookAt, targetPosition, targetLookAt;
	Vector3 up;
	float fov, near, far;

	Vector3 previousMousePosition;

	float orbitX, orbitY, distance;
};

#endif
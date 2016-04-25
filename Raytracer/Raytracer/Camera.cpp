#include "Camera.h"
#include <iostream>
#include <glut.h>


Camera::Camera() : SceneObject()
{
}

Camera::Camera(Vector3 _position, Vector3 _lookAt, Vector3 _up, float _fov, float _near, float _far) : SceneObject(_position)
{
	lookAt = _lookAt;
	targetLookAt = lookAt;
	targetPosition = _position;
	transform.up = _up;
	fov = _fov;
	near = _near;
	far = _far;

	orbitX =20;// -30; 
	orbitY = 150;//30;
	distance = 5;
}

void Camera::draw()
{
}

void Camera::update()
{
	lookAt = Vector3::Lerp( lookAt, targetLookAt, 0.2 );

	if( ( targetLookAt - lookAt ).getLength() < 0.001 )
		lookAt = targetLookAt;

	if ( Input::getMouseButton(0) )
	{
		orbitY -= (Input::mousePosition.x - Input::previousMousePosition.x)/(float)2;
		orbitX += (Input::mousePosition.y - Input::previousMousePosition.y)/(float)2;

		if (orbitX > 89)
			orbitX = 89;
		if (orbitX < -89)
			orbitX = -89;
	}
	else if ( Input::getMouseButton(1) )
	{
		distance += (Input::mousePosition.y - Input::previousMousePosition.y)/25;

		if ( distance < 0.2 )
			distance = 0.2;
	}
	else if( Input::getMouseButton(2) )
	{
		lookAt = targetLookAt = targetLookAt + transform.left * (Input::mousePosition.x - Input::previousMousePosition.x)*0.01;
		lookAt = targetLookAt = targetLookAt + transform.up * (Input::mousePosition.y - Input::previousMousePosition.y)*0.01;
	}

	transform.setPosition(lookAt);
	transform.setRotation(orbitX,orbitY,0);
	transform.translate(0,0,-distance);
}

void Camera::initializeCamera( int width, int height )
{
	glViewport(0,0,width, height);
	//std::cout << width << std::endl;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( fov, (float)width/height, near, far ); // Warning
	//glOrtho(-2,2,-2,2,0.001,100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Transform view = transform;

	view.setPosition(transform.position.x, transform.position.y, transform.position.z);

	view.setRotation(-transform.rotation.x, transform.rotation.y+180, transform.rotation.z);

	GLdouble m[16];

	Matrix ci = view.transformationMatrix.getInverse();

	for (int i = 0; i < 16; i++)
		m[i] = (GLdouble)(ci.m[i]);

	glMultMatrixd(m);

	//gluLookAt(transform.position.x, transform.position.y, transform.position.z,
		//		lookAt.x, lookAt.y, lookAt.z,
			//	transform.up.x, transform.up.y, transform.up.z);
}
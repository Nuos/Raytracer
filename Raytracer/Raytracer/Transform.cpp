#include "Transform.h"
#include <iostream>
#include <math.h>

Transform::Transform()
{
	left = Vector3( 1, 0, 0 );
	up = Vector3( 0, 1, 0 );
	forward = Vector3( 0, 0, 1 );
}

void Transform::setPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;

	calculateTransformationMatrix();
}

void Transform::setPosition(Vector3 _position)
{
	position = _position;
	calculateTransformationMatrix();
}

void Transform::translate( float x, float y, float z )
{
	Matrix t;
	t.m[12] = x;
	t.m[13] = y;
	t.m[14] = z;

	transformationMatrix = transformationMatrix * t;

	position = transformationMatrix * Vector3(0,0,0);
}


void Transform::setRotation(float x, float y, float z)
{
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;

	calculateTransformationMatrix();
}

void Transform::rotate(float angle, int axis)
{
	if (axis == 0)
	{
		Matrix rx;
		float deg = angle * 0.0174532925;

		rx.m[5] = cos(deg);
		rx.m[10] = cos(deg);
		rx.m[6] = sin(deg);
		rx.m[9] =-sin(deg);

		transformationMatrix = transformationMatrix * rx;
		forward = rx * forward;
		left = rx * left;
	}
	else if(axis == 1)
	{
		Matrix ry;
		float deg = angle * 0.0174532925;

		ry.m[0] = cos(deg);
		ry.m[10] = cos(deg);
		ry.m[2] = -sin(deg);
		ry.m[8] = sin(deg);

		transformationMatrix = transformationMatrix * ry;
		forward = ry * forward;
		left = ry * left;
	}
	else if(axis == 2)
	{
		Matrix rz;
		float deg = angle * 0.0174532925;

		rz.m[0] = cos(deg);
		rz.m[5] = cos(deg);
		rz.m[4] = sin(deg);
		rz.m[1] =-sin(deg);

		transformationMatrix = transformationMatrix * rz;
		forward = rz * forward;
		left = rz * left;
	}

	rotation.x = (-atan2(transformationMatrix.m[9],transformationMatrix.m[10]) / 3.14159265) * 180;
	rotation.y = (asin(transformationMatrix.m[8]) / 3.14159265) * 180;
	rotation.z = (-atan2(transformationMatrix.m[4],transformationMatrix.m[0]) / 3.14159265) * 180;

	up = Vector3::Cross(forward, left);

	up.normalize();
	forward.normalize();
	left.normalize();

}
void Transform::calculateTransformationMatrix()
{
	Matrix t;
	t.m[12] = position.x;
	t.m[13] = position.y;
	t.m[14] = position.z;

	Matrix ry;
	float deg = rotation.y * 0.0174532925;

	ry.m[0] = cos(deg);
	ry.m[10] = cos(deg);
	ry.m[2] = -sin(deg);
	ry.m[8] = sin(deg);

	Matrix rx;
	deg = rotation.x * 0.0174532925;

	rx.m[5] = cos(deg);
	rx.m[10] = cos(deg);
	rx.m[6] = sin(deg);
	rx.m[9] =-sin(deg);

	Matrix rz;
	deg = rotation.z * 0.0174532925;

	rz.m[0] = cos(deg);
	rz.m[5] = cos(deg);
	rz.m[4] = -sin(deg);
	rz.m[1] = sin(deg);

	transformationMatrix = t * ry * rx * rz;

	forward = ry * rx * rz * Vector3(0,0,1);
	left = ry * rx * rz * Vector3(1,0,0);
	up = Vector3::Cross(forward, left);

	forward.normalize();
	left.normalize();
	up.normalize();
}

void Transform::lookAt( Vector3 _position )
{
	Vector3 toTarget = _position - position;

	float y = atan2(toTarget.x, toTarget.z) / 0.0174532925;
	float x = -atan2(toTarget.y, sqrt(toTarget.z*toTarget.z + toTarget.x *toTarget.x))/0.0174532925;


	std::cout << "LookAt: X: " << x << " Y: " << y << std::endl;
	setRotation(x,y,0);
}

Matrix Transform::getRotationMatrix()
{
	Matrix r;

	Matrix ry;
	float deg = rotation.y * 0.0174532925;

	ry.m[0] = cos(deg);
	ry.m[10] = cos(deg);
	ry.m[2] = -sin(deg);
	ry.m[8] = sin(deg);

	Matrix rx;
	deg = rotation.x * 0.0174532925;

	rx.m[5] = cos(deg);
	rx.m[10] = cos(deg);
	rx.m[6] = sin(deg);
	rx.m[9] =-sin(deg);

	Matrix rz;
	deg = rotation.z * 0.0174532925;

	rz.m[0] = cos(deg);
	rz.m[5] = cos(deg);
	rz.m[4] = -sin(deg);
	rz.m[1] = sin(deg);

	return ry * rx * rz;
}
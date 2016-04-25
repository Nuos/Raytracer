#ifndef _MATRIX_H
#define _MATRIX_H

#include "Vector3.h"

class Matrix
{
public:
	Matrix();
	float getDeterminant();
	Matrix getInverse();
	Matrix operator+(Matrix &other);
	Matrix operator*(Matrix &other);
	Vector3 operator*(Vector3 &vector);

	float m[16]; 
};

#endif
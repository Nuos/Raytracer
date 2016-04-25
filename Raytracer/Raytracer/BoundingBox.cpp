#include "BoundingBox.h"
#include <glut.h>

BoundingBox::BoundingBox()
{
}

BoundingBox BoundingBox::operator+=(BoundingBox &other)
{
	this->min.x = Utils::min(this->min.x, other.min.x);
	this->min.y = Utils::min(this->min.y, other.min.y);
	this->min.z = Utils::min(this->min.z, other.min.z);

	this->max.x = Utils::max(this->max.x, other.max.x);
	this->max.y = Utils::max(this->max.y, other.max.y);
	this->max.z = Utils::max(this->max.z, other.max.z);

	return *this;
}

BoundingBox BoundingBox::operator+=(Vector3 &other)
{
	this->min += other;
	this->max += other;

	return *this;
}

void BoundingBox::draw( Vector3 &color, bool lineWidth, float alpha )
{
	glLineWidth( lineWidth );
	glDisable(GL_LIGHTING);
	glColor4f( color.x,color.y,color.z, alpha );
		
	glBegin(GL_LINES);

	glVertex3f(min.x, min.y, min.z);
	glVertex3f(min.x, min.y, max.z);

	glVertex3f(min.x, max.y, min.z);
	glVertex3f(min.x, max.y, max.z);

	glVertex3f(max.x, max.y, min.z);
	glVertex3f(max.x, max.y, max.z);

	glVertex3f(max.x, min.y, min.z);
	glVertex3f(max.x, min.y, max.z);



	glVertex3f(min.x, min.y, min.z);
	glVertex3f(min.x, max.y, min.z);

	glVertex3f(min.x, min.y, max.z);
	glVertex3f(min.x, max.y, max.z);

	glVertex3f(max.x, min.y, max.z);
	glVertex3f(max.x, max.y, max.z);

	glVertex3f(max.x, min.y, min.z);
	glVertex3f(max.x, max.y, min.z);



	glVertex3f(min.x, min.y, min.z);
	glVertex3f(max.x, min.y, min.z);

	glVertex3f(min.x, min.y, max.z);
	glVertex3f(max.x, min.y, max.z);

	glVertex3f(min.x, max.y, max.z);
	glVertex3f(max.x, max.y, max.z);

	glVertex3f(min.x, max.y, min.z);
	glVertex3f(max.x, max.y, min.z);

	glEnd();
}
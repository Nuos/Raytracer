#include "Light.h"
#include <glut.h>

Light::Light() : SceneObject()
{
	isArea = false;
}

Light::Light(Vector3 _position, Vector3 _color) : SceneObject(_position)
{
	color = _color;
	isArea = false;
	sampleSize = 9;
	size = 0.5;
	specularIntensity = 1;

	calculateBoundingBox();
}

void Light::draw()
{
	SceneObject::draw();

	glPushMatrix();

	GLdouble m[16];

	for (int i = 0; i < 16; i++)
	{
		m[i] = (GLdouble)( transform.transformationMatrix.m[i] );
	}

	glMultMatrixd(m);

	glDisable(GL_LIGHTING);
	glLineWidth(2);
	glColor3f( color.x, color.y, color.z );

	float gizmoSize = 0.5;

	if( isArea )
	{
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

		glBegin(GL_QUADS);

		glVertex3f(-size/2,0,-size/2);
		glVertex3f(-size/2,0, size/2);
		glVertex3f(size/2,0,size/2);
		glVertex3f(size/2,0,-size/2);

		glEnd();

		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	}
	else
	{
		glBegin(GL_LINES);

		glVertex3f(-gizmoSize,0,0);
		glVertex3f( gizmoSize,0,0);
		glVertex3f(0,-gizmoSize,0);
		glVertex3f(0,gizmoSize,0);
		glVertex3f(0,0,-gizmoSize);
		glVertex3f(0,0,gizmoSize);

		glEnd();
	}

	glEnable(GL_LIGHTING);

	glPopMatrix();
}

void Light::update()
{
}

void Light::initializeLight(int index)
{
	float diffuse[4] = {color.x, color.y, color.z, 1};
	float pos[4] = {transform.position.x, transform.position.y, transform.position.z, 1};

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_LIGHT0 + index);
	glLightfv(GL_LIGHT0 + index, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0 + index, GL_POSITION, pos);
	glEnable(GL_NORMALIZE);
}

void Light::calculateBoundingBox()
{
	boundingBox.min = transform.position - Vector3(0.5, 0.5, 0.5);
	boundingBox.max = transform.position + Vector3(0.5, 0.5, 0.5);
}

void Light::drawGUI()
{
	SceneObject::drawGUI();

	char* str = new char[30];

	float vOffset = -40;

	GUI::Label(5, 345+vOffset, "Color");

	sprintf(str, "R: %.1f", color.x ); 
	GUI::Label(26, 365+vOffset, str);

	sprintf(str, "G: %.1f", color.y ); 
	GUI::Label(26, 385+vOffset, str);

	sprintf(str, "B: %.1f", color.z ); 
	GUI::Label(26, 405+vOffset, str);

	if(GUI::ArrowLeft(5, 365+vOffset, 16))
	{
		color.x -= 0.1;
	}

	if(GUI::ArrowRight(69, 365+vOffset, 16))
	{
		color.x += 0.1;
	}

	if(GUI::ArrowLeft(5, 385+vOffset, 16))
	{
		color.y -= 0.1;
	}

	if(GUI::ArrowRight(69, 385+vOffset, 16))
	{
		color.y += 0.1;
	}

	if(GUI::ArrowLeft(5, 405+vOffset, 16))
	{
		color.z -= 0.1;
	}

	if(GUI::ArrowRight(69, 405+vOffset, 16))
	{
		color.z += 0.1;
	}

	//sprintf(str, "B: %.1f", color.z );
	if( isArea )
		GUI::Label(26, 430+vOffset, "Area");
	else
		GUI::Label(26, 430+vOffset, "Point");

	if(GUI::ArrowLeft(5, 430+vOffset, 16))
	{
		isArea = !isArea;
	}

	if(GUI::ArrowRight(69, 430+vOffset, 16))
	{
		isArea = !isArea;
	}
}
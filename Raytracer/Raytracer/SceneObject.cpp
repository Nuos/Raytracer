#include "SceneObject.h"
#include <iostream>

#include "Screen.h"

SceneObject::SceneObject()
{
	transform.position = Vector3(0, 0, 0);
}

SceneObject::SceneObject(Vector3 position)
{
	transform.setPosition( position );
}

void SceneObject::draw()
{
	updateTranslationGizmo();
}

void SceneObject::update()
{

}

void SceneObject::setPosition( float x, float y, float z )
{
	transform.setPosition( x, y, z );
	calculateBoundingBox();
}

void SceneObject::setPosition( Vector3 position )
{
	transform.setPosition( position );
	calculateBoundingBox();
}

void SceneObject::setRotation( float x, float y, float z )
{
	transform.setRotation( x, y, z );
	calculateBoundingBox();
}

void SceneObject::drawGUI()
{
	char* str = new char[30];

	int vOffset = -40;

	GUI::Label(5, 160+vOffset, "Position");

	sprintf(str, "X: %.1f", transform.position.x); 
	GUI::Label(26, 180+vOffset, str);

	sprintf(str, "Y: %.1f", transform.position.y); 
	GUI::Label(26, 200+vOffset, str);

	sprintf(str, "Z: %.1f", transform.position.z); 
	GUI::Label(26, 220+vOffset, str);

	GUI::Label(5, 250+vOffset, "Rotation");

	sprintf(str, "X: %.f", transform.rotation.x); 
	GUI::Label(26, 270+vOffset, str);

	sprintf(str, "Y: %.f", transform.rotation.y); 
	GUI::Label(26, 290+vOffset, str);

	sprintf(str, "Z: %.f", transform.rotation.z); 
	GUI::Label(26, 310+vOffset, str);

	if(GUI::ArrowLeft(5, 180+vOffset, 16))
	{
		setPosition(transform.position.x-1, transform.position.y, transform.position.z);
	}

	if(GUI::ArrowRight(69, 180+vOffset, 16))
	{
		setPosition(transform.position.x+1, transform.position.y, transform.position.z);
	}

	if(GUI::ArrowLeft(5, 200+vOffset, 16))
	{
		setPosition(transform.position.x, transform.position.y-1, transform.position.z);
	}

	if(GUI::ArrowRight(69, 200+vOffset, 16))
	{
		setPosition(transform.position.x, transform.position.y+1, transform.position.z);
	}

	if(GUI::ArrowLeft(5, 220+vOffset, 16))
	{
		setPosition(transform.position.x, transform.position.y, transform.position.z-1);
	}

	if(GUI::ArrowRight(69, 220+vOffset, 16))
	{
		setPosition(transform.position.x, transform.position.y, transform.position.z+1);

	}

	if(GUI::ArrowLeft(5, 270+vOffset, 16))
	{
		setRotation(transform.rotation.x-5, transform.rotation.y, transform.rotation.z);
	}

	if(GUI::ArrowRight(69, 270+vOffset, 16))
	{
		setRotation(transform.rotation.x+5, transform.rotation.y, transform.rotation.z);
	}

	if(GUI::ArrowLeft(5, 290+vOffset, 16))
	{
		setRotation(transform.rotation.x, transform.rotation.y-5, transform.rotation.z);
	}

	if(GUI::ArrowRight(69, 290+vOffset, 16))
	{
		setRotation(transform.rotation.x, transform.rotation.y+5, transform.rotation.z);
	}

	if( GUI::ArrowLeft(5, 310+vOffset, 16) )
	{
		setRotation(transform.rotation.x, transform.rotation.y, transform.rotation.z-5);
	}

	if(GUI::ArrowRight(69, 310+vOffset, 16))
	{
		setRotation(transform.rotation.x, transform.rotation.y, transform.rotation.z+5);
	}

	Vector3 mouseDelta = Input::mousePosition - Input::previousMousePosition;
	float delta = 0;

	if( gizmoMode == GizmoMode::TX )
		glColor3f(1,0,0);
	else
		glColor3f(0.6,0,0);

	if( GUI::ClickableLine( tStartX.x, tStartX.y, tEndX.x, tEndX.y ) )
		gizmoMode = GizmoMode::TX;

	if( gizmoMode == GizmoMode::TY )
		glColor3f(0,1,0);
	else
		glColor3f(0,0.6,0);

	if( GUI::ClickableLine( tStartY.x, tStartY.y, tEndY.x, tEndY.y ) )
		gizmoMode = GizmoMode::TY;

	if( gizmoMode == GizmoMode::TZ )
		glColor3f(0,0,1);
	else
		glColor3f(0,0,0.6);

	if( GUI::ClickableLine( tStartZ.x, tStartZ.y, tEndZ.x, tEndZ.y ) )
		gizmoMode = GizmoMode::TZ;

	if( Input::getMouseButtonUp( 0 ) )
		gizmoMode = GizmoMode::NONE;

	if( gizmoMode == GizmoMode::TX )
	{
		delta = Vector3::Dot( mouseDelta * (1 / (tEndX - tStartX).getLength()), tEndX - tStartX ) / (tEndX - tStartX).getLength();

		setPosition( transform.position + Vector3(1,0,0) * delta );
	}

	if( gizmoMode == GizmoMode::TY )
	{
		delta = Vector3::Dot( mouseDelta * (1 / (tEndY - tStartY).getLength()), tEndY - tStartY ) / (tEndY - tStartY).getLength();

		setPosition( transform.position + Vector3(0,1,0) * delta );
	}

	if( gizmoMode == GizmoMode::TZ )
	{
		delta = Vector3::Dot( mouseDelta * (1 / (tEndZ - tStartZ).getLength()), tEndZ - tStartZ ) / (tEndZ - tStartZ).getLength();

		setPosition( transform.position + Vector3(0,0,1) * delta );
	}

	//for( int i = 0; i < 18; i++ )
	//{
	//	if( GUI::ClickableLine( xTangents[i%20][0], xTangents[i%20][1], xTangents[(i+1)%20][0], xTangents[(i+1)%20][1] ) );
	//}

	//int *k;
	//glGetIntegerv(GL_NAME_STACK_DEPTH, k);
	//std::cout << *k << " " ;
}

void SceneObject::updateTranslationGizmo()
{
	glPushMatrix();

	GLdouble model[16];
	GLdouble proj[16];
	int view[4];

	glTranslatef(transform.position.x, transform.position.y, transform.position.z);
	glGetDoublev(GL_MODELVIEW_MATRIX, model);
	glGetDoublev(GL_PROJECTION_MATRIX, proj);
	glGetIntegerv(GL_VIEWPORT, view);

	GLdouble x, y, z;

	glLineWidth(1);

	Vector3 topLeft, topRight, bottomLeft;
	gluProject(0, 0, 0, model, proj, view, &x, &y, &z);

	tStartX.x = x;
	tStartX.y = g_h-y;
	tStartY.x = x;
	tStartY.y = g_h-y;
	tStartZ.x = x;
	tStartZ.y = g_h-y;

	float rotGizmoSize = 1;

	gluProject(rotGizmoSize, 0, 0, model, proj, view, &x, &y, &z);

	tEndX.x = x;
	tEndX.y = g_h-y;

	gluProject(0, rotGizmoSize, 0, model, proj, view, &x, &y, &z);

	tEndY.x = x;
	tEndY.y = g_h-y;

	gluProject(0, 0, rotGizmoSize, model, proj, view, &x, &y, &z);

	tEndZ.x = x;
	tEndZ.y = g_h-y;

	glMatrixMode(GL_MODELVIEW);

	
		glRotatef(transform.rotation.y, 0,1,0);
	glRotatef(transform.rotation.x, 1,0,0);
	glRotatef(transform.rotation.z, 0,0,1);
	

	glGetDoublev(GL_MODELVIEW_MATRIX, model);
	glGetDoublev(GL_PROJECTION_MATRIX, proj);
	glGetIntegerv(GL_VIEWPORT, view);

	const float DEG2RAD = 3.14159/180;

	for (int i=0; i <= 360; i += 20)
    {		
        float degInRad = i*DEG2RAD;

		gluProject(0.0f,cos(degInRad)*rotGizmoSize,sin(degInRad)*rotGizmoSize, model,proj, view, &x, &y, &z);
		xTangents[i/20][0] = x; xTangents[i/20][1] = g_h - y;
    }

	for (int i=0; i <= 360; i += 20)
    {		
        float degInRad = i*DEG2RAD;

		gluProject(cos(degInRad)*rotGizmoSize, 0.0f, sin(degInRad)*rotGizmoSize, model,proj, view, &x, &y, &z);
		yTangents[i/20][0] = x; yTangents[i/20][1] = g_h - y;
    }

	for (int i=0; i <= 360; i += 20)
    {		
        float degInRad = i*DEG2RAD;

		gluProject(cos(degInRad)*rotGizmoSize,sin(degInRad)*rotGizmoSize, 0.0f, model,proj, view, &x, &y, &z);
		zTangents[i/20][0] = x; zTangents[i/20][1] = g_h - y;
    }

	glMatrixMode(GL_MODELVIEW);

	glPopMatrix();
}
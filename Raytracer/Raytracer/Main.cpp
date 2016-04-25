#include <iostream>
#include <glut.h>

#include "Input.h"
#include "Scene.h"

#include "Screen.h"

//int windowWidth = 640;
//int windowHeight = 640;

void update(int value);
void draw();
void drawGUI();
void mouseFunction(int button, int state, int x, int y);
void motionFunction(int x, int y);
void passiveMotionFunction(int x, int y);
void processNormalKeys(unsigned char key, int x, int y);
void reshape(int x, int y);

Scene *scene;

bool drawLastFrame = false;

int main(int argc, char** argv)
{


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(20,20);


	glutInitWindowSize(g_w, g_h);
	glutCreateWindow("Ray Tracer | Aytek Aman");

	glutMouseFunc(mouseFunction);
	glutMotionFunc(motionFunction);
	glutPassiveMotionFunc(passiveMotionFunction);
	glutKeyboardFunc(processNormalKeys);
	glutReshapeFunc(reshape);
	glutDisplayFunc(draw);
	glutTimerFunc(66, update, 0);

	scene = new Scene();
	//glutFullScreen();
	glutMainLoop();

	return 0;
}

void update(int value)
{
	scene->update();
 
	draw();
	
	Input::update(); // Strict rule: Must be called after everything else is updated.
	glutTimerFunc(16.6, update, 0);
}

void draw()
{
	if( drawLastFrame && false ) // disabled
	{
		glClearColor(0,0,0,1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		scene->raytracer->drawFrameBuffer();
		glutSwapBuffers();
	}
	else if ( !scene->isRendering )
	{
		glClearColor(0,0,0,1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		scene->draw( g_w, g_h );
		drawGUI();
		glutSwapBuffers();
	}
}

void drawGUI()
{
	GUI::initializeGUI( false );

	scene->drawGUI( g_w, g_h );

	GUI::end();
}

void passiveMotionFunction(int x, int y)
{
	Input::onPassiveMotionFunction( x, y );
}

void motionFunction(int x, int y)
{
	Input::onMotionFunction( x, y );
}

void mouseFunction(int button, int state, int x, int y)
{
	if (  button == 0 && state == GLUT_DOWN )
	{
		GUI::initializeGUI( true );

		scene->drawGUI( g_w, g_h );

		GUI::end();
		GUI::processPicks();	
	

		//GUI::initializeGUI( false );

		//scene->drawGUI( windowWidth, windowHeight );

		//GUI::end();
	}

	if( !GUI::absorbeEvents )
		Input::onMouseFunction( button, state, x, y );

	GUI::absorbeEvents = false;
}

void processNormalKeys(unsigned char key, int x, int y) 
{
	if(key == 'C' || key == 'c')
	{
		scene->activeSceneObject = NULL;
	}
	else if(key == 'D' || key == 'd')
	{
		scene->deleteActiveSceneObject();
	}
	else if(key == 'F' || key == 'f')
	{
		if( scene->activeSceneObject == NULL )
			scene->camera->targetLookAt = Vector3( 0, 0, 0 );
		else
			scene->camera->targetLookAt = scene->activeSceneObject->transform.position;
	}
	else if(key == 'H' || key == 'h')
	{
		scene->hideMeshes = !scene->hideMeshes;
	}
	else if(key == 'R' || key == 'r')
	{
		drawLastFrame = !drawLastFrame;
	}
}

void reshape(int x, int y)
{
	g_w = x;
	g_h = y;
}
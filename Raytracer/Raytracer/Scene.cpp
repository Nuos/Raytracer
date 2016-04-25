#include "Scene.h"

//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

#include "Screen.h";

Scene::Scene()
{
	treeDepth = 20;
	threshold = 50;

	camera = new Camera( Vector3(0, 0, 0), Vector3(0, 0, 0) );
	mesh = new Mesh("bunny.txt");
	
	Mesh* plane = new Mesh("plane.txt");
	plane->material->reflectivity = 0.5;
	//mesh2 = new Mesh("bunny.txt");
	//Mesh* mesh3 = new Mesh("bunny.txt");

	meshList = new List<Mesh*>();
	lightList = new List<Light*>();

	light = new Light( Vector3(5, 5, 1), Vector3(0.1, 0.1, 0.5) );
	lightList->add( light );
	//light->isArea = true;
	light = new Light( Vector3(-5, 5, 1), Vector3(1, 0.6, 0.3) );
	lightList->add( light );

	plane->transform.setPosition(0,mesh->boundingBox.min.y,0);
	//mesh3->transform.setRotation(11,11,55);
	plane->calculateBoundingBox();

	meshList->add( mesh );
	meshList->add( plane );
	//meshList->add(mesh2);
	//meshList->add(mesh3);


	recalculateBoundingBox();
	sceneTree = new BSPTree( meshList, boundingBox, 20, 50 );

	isWireframe = false;
	isBoundingBoxEnabled = false;
	isRendering = false;
	hideMeshes = false;

	raytracer = new Raytracer(this);

	activeSceneObject = NULL;

	depth = 0;
}

void Scene::draw( int width, int height )
{
	camera->initializeCamera( width, height );

	for( int i = 0; i < lightList->size(); i++ )
		lightList->get( i )->initializeLight( i );

	if ( isWireframe )
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	else
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	if ( isBoundingBoxEnabled )
	{
		for ( int i = 0; i < meshList->size(); i++ )
			meshList->get(i)->boundingBox.draw();

		for ( int i = 0; i < lightList->size(); i++ )
			lightList->get(i)->boundingBox.draw();
	}

	if( !hideMeshes )
		for ( int i = 0; i < meshList->size(); i++ )
			meshList->get(i)->draw();

	for ( int i = 0; i < lightList->size(); i++ )
		lightList->get(i)->draw();

	if( activeSceneObject && !hideMeshes )
	{
		activeSceneObject->boundingBox.draw( Vector3( 1, 1, 1 ) );
	}
	//boundingBox.draw();

	depth += 0.021;
	//octree->draw(depth);
	if( sceneTree != NULL )
		sceneTree->draw();

	//glDisable(GL_LIGHTING);
	//glColor3f(0.5,0.5,0.5);
	//glLineWidth(1);
	//for (int i = 0; i <= 20; i+=2)
	//{
	//	glBegin(GL_LINES);
	//	glVertex3f(-10,0,i-10);
	//	glVertex3f(10,0,i-10);
	//	glEnd();

	//	glBegin(GL_LINES);
	//	glVertex3f(i-10,0,-10);
	//	glVertex3f(i-10,0,10);
	//	glEnd();
	//}

	//glLineWidth(3);
	//glColor3f(1,0,0);
	//glBegin(GL_LINES);
	//glVertex3f(0,0,0);
	//glVertex3f(1,0,0);
	//glEnd();

	//glColor3f(0,1,0);
	//glBegin(GL_LINES);
	//glVertex3f(0,0,0);
	//glVertex3f(0,1,0);
	//glEnd();

	//glColor3f(0,0,1);
	//glBegin(GL_LINES);
	//glVertex3f(0,0,0);
	//glVertex3f(0,0,1);
	//glEnd();
	glEnable(GL_LIGHTING);
}

void Scene::drawGUI( int width, int height )
{
	if( activeSceneObject != NULL )
		activeSceneObject->drawGUI();

	if( sceneTree != NULL )
		sceneTree->drawGUI();

	//if ( isWireframe )
	//{
	//	if ( GUI::Button(5,5,100,20, "Wireframe ON") )
	//		isWireframe = !isWireframe;
	//}
	//else if ( GUI::Button(5,5,100,20, "Wireframe OFF") )
	//		isWireframe = !isWireframe;

	if ( GUI::Button( 5, 5, 80, 20, "Bunny" ) )
		addMesh( new Mesh( "bunny.txt" ) );

	if ( GUI::Button(5,30,80,20, "Dragon") )
		addMesh( new Mesh( "dragon.txt", -1 ) );

	if ( GUI::Button(5,55,80,20, "Plane") )
		addMesh( new Mesh( "plane.txt" ) );

	if ( GUI::Button(5,80,80,20, "Light") )
		addLight( new Light( Vector3( 0, 5, 0 ), Vector3( 1, 1, 1 ) ) );

	int right = g_w - 105;

	int top = g_h - 80;

	char* str = new char[30];

	GUI::Label(right, top - 90, "Tree depth");

	sprintf( str, "%d", treeDepth ); 
	GUI::Label(right + 21, top - 70, str);

	GUI::Label(right, top - 50, "Threshold");

	sprintf( str, "%d", threshold ); 
	GUI::Label(right + 21, top - 30, str);

	if(GUI::ArrowLeft(right, top - 70, 16))
	{
		treeDepth--;
	}

	if(GUI::ArrowRight(right + 64, top - 70, 16))
	{
		treeDepth++;
	}

	if(GUI::ArrowLeft(right, top - 30, 16))
	{
		threshold -= 5;
	}

	if(GUI::ArrowRight(right + 64, top - 30, 16))
	{
		threshold += 5;
	}


	if ( GUI::Button(right, top, 32, 20, "Oc") )
	{
		if( sceneTree != NULL )
			delete sceneTree;

		recalculateBoundingBox();

		sceneTree = new OcTree( meshList, boundingBox, treeDepth, threshold );
	}

	//top += 25;

	if ( GUI::Button(right+34, top, 32,20, "BSP") )
	{
		if( sceneTree != NULL )
			delete sceneTree;

		recalculateBoundingBox();

		sceneTree = new BSPTree( meshList, boundingBox, treeDepth, threshold );
	}

	//top += 25;

	if ( GUI::Button(right+68, top, 32,20, "kD") )
	{
		if( sceneTree != NULL )
			delete sceneTree;

		recalculateBoundingBox();

		sceneTree = new BSPTree( meshList, boundingBox, treeDepth, threshold, true, 0.05 );
	}

	top += 30;

	if ( raytracer->useFalseColor )
	{
		if ( GUI::Button(right,top,100,20, "P. Color ON") )
			raytracer->useFalseColor = !raytracer->useFalseColor;
	}
	else if ( GUI::Button(right,top,100,20, "P. Color OFF") )
			raytracer->useFalseColor = !raytracer->useFalseColor;

	top += 25;

	if ( GUI::Button(right, top, 100, 20, "Render") )
	{
		//for( int d = 10; d < 40; d++ )
		//{
		//	if( sceneTree != NULL )
		//		delete sceneTree;

		//	sceneTree = new BSPTree( meshList, boundingBox, d, 10 );

		//	raytracer->raytrace();
		//}

		if( sceneTree != NULL )
			raytracer->raytrace();
	}
}

void Scene::update()
{
	if ( Input::getMouseButtonDown(2) )
		isRendering = false;

	camera->update();

	if ( Input::getMouseButtonDown( 0 )  )
	{
		Vector3 mousePos = Input::mousePosition;

		activeSceneObject = pickObject( mousePos.x, mousePos.y );
	}
}

void Scene::deleteActiveSceneObject()
{
	if( activeSceneObject != NULL )
	{
		if ( meshList->remove( dynamic_cast<Mesh*>( activeSceneObject ) ) )
		{
			delete dynamic_cast<Mesh*>( activeSceneObject );
			activeSceneObject = NULL;
		}

		if ( lightList->remove( dynamic_cast<Light*>( activeSceneObject ) ) )
		{
			activeSceneObject = NULL;
			Light::resetLights();
		}

		recalculateBoundingBox();
	}
}

void Scene::recalculateBoundingBox()
{
	if( meshList->size() == 0 )
	{
		boundingBox.min = Vector3( 0, 0, 0 );
		boundingBox.max = Vector3( 0, 0, 0 );
	}
	else
	{
		boundingBox = meshList->get(0)->boundingBox;

		for ( int i = 1; i < meshList->size(); i++ )
			boundingBox += meshList->get(i)->boundingBox;
	}
}

void Scene::addLight( Light* light )
{
	lightList->add( light );
}

void Scene::addMesh( Mesh* mesh )
{
	meshList->add( mesh );
	recalculateBoundingBox();
	activeSceneObject = mesh;
}

SceneObject* Scene::pickObject( int x, int y )
{
	Vector3 camPos = camera->transform.position;
	Vector3 topLeft = camPos + camera->transform.forward + camera->transform.left * tan(camera->fov* 0.0174532925*0.5)*((float)g_w/g_h) + camera->transform.up* tan(camera->fov* 0.0174532925*0.5) ;
	Vector3 topRight = camPos + camera->transform.forward + camera->transform.left*-1* tan(camera->fov* 0.0174532925*0.5)*((float)g_w/g_h) + camera->transform.up* tan(camera->fov* 0.0174532925*0.5) ;
	Vector3 bottomLeft = camPos + camera->transform.forward + camera->transform.left* tan(camera->fov* 0.0174532925*0.5)*((float)g_w/g_h)  + camera->transform.up*-1* tan(camera->fov* 0.0174532925*0.5) ;
	Vector3 horizontalIncrement = ( topRight - topLeft ) * (1 / (float)(g_w) );
	Vector3 verticalIncrement = ( bottomLeft - topLeft ) * (1 / (float)(g_h) );
	topLeft += verticalIncrement * 0.5;
	topLeft += horizontalIncrement * 0.5;

	Ray ray;
	ray.origin = camera->transform.position;
	ray.direction = topLeft + horizontalIncrement * x + verticalIncrement * y - camera->transform.position;
	ray.direction.normalize();

	float temp = 1000000;
	int index = -1;
	SceneObject* closest = NULL;

	for( int i = 0; i < meshList->size(); i++ )
	{
		float t;
		if( ray.intersects( meshList->get( i )->boundingBox, t ) )
			if( t < temp )
			{
				temp = t;
				closest = meshList->get( i );
			}
	}

	for( int i = 0; i < lightList->size(); i++ )
	{
		float t;
		if( ray.intersects( lightList->get( i )->boundingBox, t ) )
			if( t < temp )
			{
				temp = t;
				closest = lightList->get( i );
			}
	}

	return closest;
}
#include "Raytracer.h"
#include "Scene.h" // wtf?

#include "Screen.h"

int Raytracer::intersectionTestCount = 0;

Raytracer::Raytracer(Scene* _scene)
{
	scene = _scene;
	height = g_h;
	width = g_w;

	useFalseColor = false;

	unitLights = NULL;
}

Raytracer::~Raytracer()
{
}

void Raytracer::setResolution(int _width, int _height)
{
	width = _width;
	height = _height;
}

float average = 0;

void Raytracer::raytrace()
{
	height = g_h;
	width = g_w;

	//frameBuffer = new Vector3*[height];

	//for( int i = 0; i < width; i++ )
	//{
	//	frameBuffer[i] = new Vector3[width];
	//}

	scene->isRendering = true;

	clock_t init, final;
	init=clock();

	initializeScreen();
	calculatePerspectiveBounds();

	initializeLights();

	Vector3 camTarget;
	average = 0;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			Vector3 camTarget = topLeft + verticalIncrement * i + horizontalIncrement * j;
			Ray ray;
			ray.origin = camPos;
			ray.direction = (camTarget - camPos);
			ray.direction.normalize();

			Vector3 color = raycastToScene( ray, 5 );

			glColor3f( color.x, color.y, color.z );

			if ( useFalseColor )
			{
				float d = intersectionTestCount / 192.0;
				Vector3 density = Vector3::Lerp( Vector3(0,0,0), Vector3(1,1,1), d );

				average += intersectionTestCount / (float)( width * height );

				if (density.z < 0)
					density.z = 0;
				if (density.x > 1)
					density.x = 1;

				glColor3f( density.x, density.y, density.z );

				
			}

			//frameBuffer[j][i] = color;

			intersectionTestCount = 0;

			glBegin(GL_POINTS);
			glVertex2i(j, i+1);
			glEnd();

			if (i % 39 == 0 && j == width - 1 )
			{
				glColor3f(1,1,1);
				glBegin(GL_LINES);
				glVertex2i(0, i+2);
				glVertex2i(width, i+2);
				glEnd();
				glFlush();
			}
		}
	}

	final=clock()-init;

	//ofstream myfile;
	//myfile.open ( "results.txt", ios_base::app );
	//std::cout << "Tree depth: " << scene->sceneTree->getMaxDepth() << ", triangle count threshold: " << scene->sceneTree->getThreshold() << std::endl;
	//myfile << "Tree depth: " << scene->sceneTree->getMaxDepth() << ", triangle count threshold: " << scene->sceneTree->getThreshold() << std::endl;
	std::cout << "Scene is rendered in " << (float)final / ((float)CLOCKS_PER_SEC) << " seconds" << std::endl;
	//myfile << "Scene is rendered in " << (float)final / ((float)CLOCKS_PER_SEC) << " seconds" << std::endl;
	//myfile.close();

	if ( useFalseColor )
		std::cout << "Intersection test count per pixel: " << average << std::endl;

	glDrawBuffer(GL_BACK);
}

void Raytracer::drawFrameBuffer()
{
	initializeScreen();

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			glColor3f( frameBuffer[j][i].x, frameBuffer[j][i].y, frameBuffer[j][i].z );
			glBegin(GL_POINTS);
			glVertex2i(j, i+1);
			glEnd();
		}
	}

	glFlush();
	glDrawBuffer(GL_BACK);
}

Vector3 Raytracer::raycastToScene( Ray &ray, int currentDepth )
{
	if( currentDepth == 0 )
		return Vector3(0, 0, 0);

	Vector3 color(0, 0, 0);

	Vector3 intersection, normal, uv;
	Material *material;

	if( scene->sceneTree->raycast( ray, intersection, normal, material ) )
	{
		Ray newRay;
		newRay.origin = intersection + 0.01 * normal;
		newRay.direction = ray.direction - 2 * normal * Vector3::Dot( ray.direction, normal );

		if( material->reflectivity > 0.0001 )
			color += material->reflectivity * raycastToScene( newRay, currentDepth - 1 );

		color = color + (1 - material->reflectivity - material->transparency ) * raycastToLights( newRay.origin, normal, material, uv );

		return color;
	}
	else return Vector3(0, 0, 0); // or ambient
}

Vector3 Raytracer::raycastToLights( Vector3 &origin, Vector3 &normal, Material *&material, Vector3 &uv )
{
	Vector3 color = Vector3( 0, 0, 0 );

	Vector3 intersection;
	Vector3 dummyVector;
	Material* dummyMaterial;

	for( int i = 0; i < unitLightCount; i++ )
	{
		Ray newRay;
		newRay.origin = origin;
		newRay.direction = unitLights[i].position - origin;
		newRay.direction.normalize();

		Vector3 toCamera = scene->camera->transform.position - origin;
		toCamera.normalize();

		if( scene->sceneTree->raycast( newRay, intersection, dummyVector, dummyMaterial ) )
		{
			Vector3 toLight = intersection - newRay.origin;
			Vector3 toLightReal = unitLights[i].position - newRay.origin;

			if( toLight.getSquaredLength() > toLightReal.getSquaredLength() )
			{
				float intensity = Vector3::Dot( newRay.direction, normal );

				if( intensity > 0 )
				{
					Vector3 addedColor = intensity * unitLights[i].color;

					addedColor.x *= material->color.x;
					addedColor.y *= material->color.y;
					addedColor.z *= material->color.z;

					color += addedColor;

					Vector3 reflectedLightVector = -1 * newRay.direction - 2 * normal * Vector3::Dot(-1 * newRay.direction, normal);

					float specular;

					if( ( specular = Vector3::Dot( reflectedLightVector, toCamera ) ) > 0 )
					{
						specular = pow( specular, 100 );
						specular *= unitLights[i].specularIntensity;

						color += specular;
					}
				}
			}
		}
		else
		{
			float intensity = Vector3::Dot( newRay.direction, normal );

			if( intensity > 0 )
			{
				Vector3 addedColor = intensity * unitLights[i].color;

				addedColor.x *= material->color.x;
				addedColor.y *= material->color.y;
				addedColor.z *= material->color.z;

				color += addedColor;

				Vector3 reflectedLightVector = -1 * newRay.direction - 2 * normal * Vector3::Dot(-1 * newRay.direction, normal);

				float specular;

				if( ( specular = Vector3::Dot( reflectedLightVector, toCamera ) ) > 0 )
				{
					specular = pow( specular, 100 );
					specular *= unitLights[i].specularIntensity;

					color += specular;
				}
			}
		}
	}

	return color;
}

void Raytracer::calculatePerspectiveBounds()
{
	camPos = scene->camera->transform.position;
	topLeft = camPos + scene->camera->transform.forward + scene->camera->transform.left * tan(scene->camera->fov* 0.0174532925*0.5)*((float)g_w/g_h) + scene->camera->transform.up* tan(scene->camera->fov* 0.0174532925*0.5) ;
	topRight = camPos + scene->camera->transform.forward + scene->camera->transform.left*-1* tan(scene->camera->fov* 0.0174532925*0.5)*((float)g_w/g_h) + scene->camera->transform.up* tan(scene->camera->fov* 0.0174532925*0.5) ;
	bottomLeft = camPos + scene->camera->transform.forward + scene->camera->transform.left* tan(scene->camera->fov* 0.0174532925*0.5)*((float)g_w/g_h)  + scene->camera->transform.up*-1* tan(scene->camera->fov* 0.0174532925*0.5) ;
	horizontalIncrement = ( topRight - topLeft ) * (1 / (float)(width) );
	verticalIncrement = ( bottomLeft - topLeft ) * (1 / (float)(height) );
	topLeft += verticalIncrement * 0.5;
	topLeft += horizontalIncrement * 0.5;
}

void Raytracer::initializeScreen()
{
	glDrawBuffer(GL_FRONT_AND_BACK);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,g_w,g_h,0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
}

void Raytracer::initializeLights()
{
	if( unitLights != NULL )
		delete unitLights;

	unitLightCount = 0;

	for( int i = 0; i < scene->lightList->size(); i++ )
	{
		if( scene->lightList->get( i )->isArea )
			unitLightCount += scene->lightList->get( i )->sampleSize * scene->lightList->get( i )->sampleSize;
		else
			unitLightCount++;
	}

	unitLights = new UnitLight[unitLightCount];

	int index = 0;

	for( int i = 0; i < scene->lightList->size(); i++ )
	{
		Light* light = scene->lightList->get( i );

		if( light->isArea )
		{
			float step = light->size / ( light->sampleSize - 1 );

			for( int j = 0; j < light->sampleSize; j++ )
			{
				for( int k = 0; k < light->sampleSize; k++ )
				{
					Vector3 localPosition;
					localPosition.x = j * step - light->size / 2;
					localPosition.z = k * step - light->size / 2;
					localPosition.y = 0;

					unitLights[index].position = light->transform.transformationMatrix * localPosition;
					unitLights[index].color = light->color * ( 1.0 / ( light->sampleSize * light->sampleSize ) );
					unitLights[index].specularIntensity = light->specularIntensity * ( 1.0 / ( light->sampleSize * light->sampleSize ) );

					index++;
				}
			}
			
		}
		else
		{
			unitLights[index].position = light->transform.position;
			unitLights[index].color = light->color;
			unitLights[index].specularIntensity = light->specularIntensity;

			index++;
		}
	}
}
#include "SPTree.h"

int SPTree::rayID = 0;

SPTree::SPTree( List<Mesh*> *&_meshList, BoundingBox _boundingBox, int _childNodeCount, int _maxDepth, int _threshold )
{
	animDelta = 0.025;

	meshList = _meshList;
	boundingBox = _boundingBox;

	childNodeCount = _childNodeCount;
	maxDepth =  _maxDepth;
	threshold = _threshold;

	cout << "Tree construction has started." << endl;

	initializeRoot();
}

SPTree::~SPTree()
{
	//std::cout << "Deletion started" << std::endl;

	delete root;

	//std::cout << "Tree is deleted" << std::endl;

	for (int i = 0; i < totalFaceCount; i++)
		delete faces[i];

	//std::cout << "Faces are deleted" << std::endl;

	delete[] faces;

	//std::cout << "Face array deleted" << std::endl;

	//delete name; !!
}

void SPTree::draw()
{
	if( showTree )
	{
		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		drawingDepth += animDelta;
		drawNode( root, 0 );

		glDisable (GL_BLEND);
	}
}

void SPTree::drawGUI()
{
	int right = g_w - 105;
	int top = 5;

	if ( GUI::Button(right, top, 48, 20, "Start") )
	{
		resumeAnimation();
	}

	//top += 25;

	if ( GUI::Button(right+52, top, 48,20, "Stop") )
	{
		stopAnimation();
	}

	top += 25;

	if ( GUI::Button(right, top, 48,20, "Show") )
	{
		setVisualisationEnabled( !showTree );
	}

	if ( GUI::Button(right+52, top, 48,20, "Reset") )
	{
		resetAnimation();
	}
}

void SPTree::drawNode( SPTreeNode *&node, int depth )
{
	if ( depth > drawingDepth )
		return;

	if( node->childNodes[0] != NULL )
		for( int i = 0; i < childNodeCount; i++ )
			drawNode( node->childNodes[i], depth + 1 );

	node->boundingBox.draw( Vector3( 0, 0.8, 0 ), 1, 0.6 );
}

//void SPTree::subdivide( SPTreeNode *&node, int currentDepth )
//{
//}

//bool SPTree::raycastNode( SPTreeNode *&node, Ray &ray, Vector3 &intersection, Vector3 &normal )
//{
//	return true;
//}

void SPTree::initializeRoot()
{
	totalFaceCount = 0;
	for ( int j = 0; j < meshList->size(); j++)
		totalFaceCount += meshList->get(j)->faceCount;

	faces = new Face*[totalFaceCount];
	for (int i = 0; i < totalFaceCount; i++)
		faces[i] = new Face();

	root = new SPTreeNode( childNodeCount, totalFaceCount );
	root->boundingBox = boundingBox;

	int index = 0;

	for (int j = 0; j < meshList->size(); j++)
	{
		int faceCount = meshList->get(j)->faceCount;
		Matrix rotationMatrix = meshList->get(j)->transform.getRotationMatrix();

		for ( int i = 0; i < faceCount; i++ )
		{
			faces[index]->vertices[0] = meshList->get(j)->transform.transformationMatrix * meshList->get(j)->vertices[3*i];
			faces[index]->vertices[1] = meshList->get(j)->transform.transformationMatrix * meshList->get(j)->vertices[3*i+1];
			faces[index]->vertices[2] = meshList->get(j)->transform.transformationMatrix * meshList->get(j)->vertices[3*i+2];

			faces[index]->normals[0] = rotationMatrix * meshList->get(j)->normals[3*i];
			faces[index]->normals[1] = rotationMatrix * meshList->get(j)->normals[3*i+1];
			faces[index]->normals[2] = rotationMatrix * meshList->get(j)->normals[3*i+2];

			faces[index]->planeNormal = Vector3::Cross( faces[index]->vertices[1] - faces[index]->vertices[0], faces[index]->vertices[2] - faces[index]->vertices[1] );
			faces[index]->planeNormal.normalize();
			
			faces[index]->material = meshList->get(j)->material;

			root->faceList->add( faces[index] );
			faces[index]->mailbox = -1;

			index++;
		}
	}
}
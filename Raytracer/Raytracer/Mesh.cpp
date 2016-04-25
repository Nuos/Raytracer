#include "Mesh.h"

Mesh::Mesh() : SceneObject()
{
	faceCount = 0;
	material = new Material();
}

Mesh::Mesh( char* fileName, int _normalFactor ) : SceneObject()
{
	normalFactor = _normalFactor;

	loadFromFile( fileName );
	centerVertices();
	material = new Material();
}

Mesh::~Mesh()
{
	delete[] vertices;
	delete[] normals;
	delete material;
}

void Mesh::draw()
{
	SceneObject::draw();

	glPushMatrix();

	GLdouble m[16];

	for (int i = 0; i < 16; i++)
	{
		m[i] = (GLdouble)( transform.transformationMatrix.m[i] );
	}

	glMultMatrixd(m);

	float diffuse[4] = {material->color.x, material->color.y, material->color.z, 1};
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);

	glEnable(GL_LIGHTING);
	glColor3f(1,1,1);
	glBegin(GL_TRIANGLES);

	for (int i = 0; i < faceCount; i++)
	{
		glNormal3f(normals[3*i].x,normals[3*i].y,normals[3*i].z);
		glVertex3f(vertices[3*i].x,vertices[3*i].y,vertices[3*i].z);
		
		glNormal3f(normals[3*i+1].x,normals[3*i+1].y,normals[3*i+1].z);
		glVertex3f(vertices[3*i + 1].x,vertices[3*i + 1].y,vertices[3*i + 1].z);
		
		glNormal3f(normals[3*i+2].x,normals[3*i+2].y,normals[3*i+2].z);
		glVertex3f(vertices[3*i + 2].x,vertices[3*i + 2].y,vertices[3*i + 2].z);	
	}

	glEnd();
	glPopMatrix();
}

void Mesh::drawGUI()
{
	SceneObject::drawGUI();

	char* str = new char[30];

	float vOffset = -40;

	GUI::Label(5, 345+vOffset, "Material");

	sprintf(str, "R: %.1f", material->color.x ); 
	GUI::Label(26, 365+vOffset, str);

	sprintf(str, "G: %.1f", material->color.y ); 
	GUI::Label(26, 385+vOffset, str);

	sprintf(str, "B: %.1f", material->color.z ); 
	GUI::Label(26, 405+vOffset, str);

	if(GUI::ArrowLeft(5, 365+vOffset, 16))
	{
		material->color.x -= 0.1;
	}

	if(GUI::ArrowRight(69, 365+vOffset, 16))
	{
		material->color.x += 0.1;
	}

	if(GUI::ArrowLeft(5, 385+vOffset, 16))
	{
		material->color.y -= 0.1;
	}

	if(GUI::ArrowRight(69, 385+vOffset, 16))
	{
		material->color.y += 0.1;
	}

	if(GUI::ArrowLeft(5, 405+vOffset, 16))
	{
		material->color.z -= 0.1;
	}

	if(GUI::ArrowRight(69, 405+vOffset, 16))
	{
		material->color.z += 0.1;
	}

	GUI::Label(5, 430+vOffset, "Reflectivity");

	sprintf(str, "%.1f", material->reflectivity ); 
	GUI::Label(26, 450+vOffset, str);

	if(GUI::ArrowLeft(5, 450+vOffset, 16))
	{
		material->reflectivity -= 0.1;
	}

	if(GUI::ArrowRight(69, 450+vOffset, 16))
	{
		material->reflectivity += 0.1;
	}
}

void Mesh::update()
{
}

void Mesh::centerVertices()
{
	Vector3 delta = 0.5 * ( boundingBox.max + boundingBox.min );

	for ( int i = 0; i < faceCount * 3; i++)
	{
		vertices[i] = vertices[i] - delta;
	}

	calculateBoundingBox();
}

void Mesh::calculateBoundingBox()
{
	boundingBox.min = Vector3(1000, 1000, 1000);
	boundingBox.max = Vector3(-1000, -1000, -1000);

	for ( int i = 0; i < faceCount*3; i++)
	{
		Vector3 temp = transform.transformationMatrix *  vertices[i];

		if ( boundingBox.min.x > temp.x )
			boundingBox.min.x = temp.x;
		if ( boundingBox.max.x < temp.x )
			boundingBox.max.x = temp.x;

		if ( boundingBox.min.y > temp.y )
			boundingBox.min.y = temp.y;
		if ( boundingBox.max.y < temp.y )
			boundingBox.max.y = temp.y;

		if ( boundingBox.min.z > temp.z )
			boundingBox.min.z = temp.z;
		if ( boundingBox.max.z < temp.z )
			boundingBox.max.z = temp.z;
	}

}

void Mesh::loadFromFile(char* fileName)
{
	ifstream inFile;
	inFile.open(fileName);

	int vertexCount;

	inFile >> vertexCount;
	std::cout << "Loading from .obj file" << endl;
	Vector3 *temp = new Vector3[vertexCount];

	for ( int i = 0; i < vertexCount; i++)
	{
		inFile >> temp[i].x;
		
		inFile >> temp[i].y;
		inFile >> temp[i].z;
		temp[i].x *= 10;
		temp[i].y *= 10;
		temp[i].z *= 10;
	}

	inFile >> faceCount;
	vertices = new Vector3[3*faceCount];
	normals = new Vector3[3*faceCount];

	int index;

	int *ref = new int[3*faceCount];
	Vector3 *vecRef = new Vector3[vertexCount];

	for (int i = 0; i < faceCount; i++)
	{
		int a,b,c;

		inFile >> a;
		vertices[3*i] = temp[a];
		ref[3*i] = a;

		inFile >> b;
		vertices[3*i+1] = temp[b];
		ref[3*i+1] = b;

		inFile >> c;
		vertices[3*i+2] = temp[c];
		ref[3*i+2] = c;

		Vector3 normal0 = Vector3::Cross(vertices[3*i] - vertices[3*i + 2], vertices[3*i+1] - vertices[3*i + 0]);
		normal0.normalize();

		vecRef[a] = vecRef[a] + normal0;
		
		Vector3 normal1 = Vector3::Cross(vertices[3*i + 1] - vertices[3*i + 0], vertices[3*i + 2] - vertices[3*i + 1]);
		normal1.normalize();

		vecRef[b] = vecRef[b] + normal1;

		Vector3 normal2 = Vector3::Cross(vertices[3*i + 2] - vertices[3*i + 1], vertices[3*i] - vertices[3*i + 2]);
		normal2.normalize();

		vecRef[c] = vecRef[c] + normal2;

		inFile >> index;
	}

	for (int i = 0; i < vertexCount; i++)
	{
		vecRef[i].normalize();
	}

	for (int i = 0; i < faceCount; i++)
	{
		normals[3*i    ] = normalFactor * vecRef[ref[3*i]];
		normals[3*i + 1] = normalFactor * vecRef[ref[3*i+1]];
		normals[3*i + 2] = normalFactor * vecRef[ref[3*i+2]];
	}

	calculateBoundingBox();

	delete[] temp;
	delete[] ref;
	delete[] vecRef;
}
#include "GUI.h"

#include "Screen.h"

GUI::GUI()
{
}

void GUI::end()
{
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	GUI::selectedID = -1;
	GUI::lastGivenID = 0;

}

bool GUI::Button(int x, int y, int width, int height, char *string)
{
	glPushName(GUI::lastGivenID++);
	glLineWidth(1);
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glColor3f(0.1,0.1,0.1);
	glBegin(GL_QUADS);
		
	glVertex2i(x,y);
	glVertex2i(x+width ,y);
	glVertex2i(x+width ,y + height);
	glVertex2i(x,y + height);
	glEnd();

	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
	if(GUI::selectedID == (lastGivenID - 1))
		glColor3f(0,1,1);
	else
		glColor3f(0,0.35,0.35);
	glBegin(GL_QUADS);
	glVertex2i(x,y);
	glVertex2i(x+width ,y);
	glVertex2i(x+width ,y+height);
	glVertex2i(x,y+height);
	glEnd();

	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(1,1,1);

	int count = 0;
	int l = 7;
	char *c;
	for (c = string; *c != '\0'; c++) 
	{
		count++;
	}

	x = x + width / 2;

	x = x - l *count *0.5;

	glRasterPos3f(x, y+14,0.2);
	
	for (c = string; *c != '\0'; c++) 
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
	}

	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);

	if(GUI::selectedID == (lastGivenID - 1))
		return true;
	else 
		return false;
}

void GUI::Label(int x, int y, char *string) 
{ 
	glPushName(-10);

	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(1,1,1);

	y += 12;

	glRasterPos3f(x, y, 0.2);
	
	char *c;

	for (c = string; *c != '\0'; c++) 
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
	}

	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
}

bool GUI::ArrowLeft(int x, int y, int size)
{
	glPushName(GUI::lastGivenID++);



	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glColor3f(0.1,0.1,0.1);
	glBegin(GL_TRIANGLES);
	glVertex2i(x+size ,y);
	glVertex2i(x,y+size/2);
	glVertex2i(x+size ,y+size);
	glEnd();

	glColor3f(1,1,1);
	glLineWidth(1);
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_TRIANGLES);
	glVertex2i(x+size ,y);
	glVertex2i(x,y+size/2);
	glVertex2i(x+size ,y+size);
	glEnd();

	if(GUI::selectedID == (lastGivenID - 1))
		return true;
	else 
		return false;
}

bool GUI::ArrowRight(int x, int y, int size)
{
	glPushName(GUI::lastGivenID++);

	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glColor3f(0.1,0.1,0.1);
	glBegin(GL_TRIANGLES);
	glVertex2i(x,y);
	glVertex2i(x+size ,y+size/2);
	glVertex2i(x,y+size);
	glEnd();
	glColor3f(1,1,1);
	glLineWidth(1);

	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_TRIANGLES);
	glVertex2i(x,y);
	glVertex2i(x+size ,y+size/2);
	glVertex2i(x,y+size);
	glEnd();
	if(GUI::selectedID == (lastGivenID - 1))
		return true;
	else 
		return false;
}

bool GUI::ClickableLine(int x1, int y1, int x2, int y2)
{
	glLineWidth(3);
	//glDisable(GL_DEPTH_TEST);
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
	glPushName(GUI::lastGivenID++);
	glBegin(GL_LINES);
	glVertex2i(x1,y1);
	glVertex2i(x2,y2);
	glEnd();
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
	//glEnable(GL_DEPTH_TEST);

	if(GUI::selectedID == (lastGivenID - 1))
		return true;
	else 
		return false;
}

void GUI::initializeGUI( bool select )
{
	GLint vpArray[4];

	if( select )
	{
		glSelectBuffer(4096, pickBuffer);
		glRenderMode(GL_SELECT);
		glInitNames();
	}

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	if( select )
	{
		glGetIntegerv(GL_VIEWPORT, vpArray);
		gluPickMatrix(GLdouble(Input::mousePosition.x), GLdouble(vpArray[3] - Input::mousePosition.y), 10, 10, vpArray);
	}

	gluOrtho2D(0,g_w,g_h,0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	
}

void GUI::processPicks()
{
	int nPicks = glRenderMode( GL_RENDER );

		if(nPicks)
		{
			//std::cout << "pick";
			GLuint objID, *ptr;
			ptr = pickBuffer;
			int selectedID;
			int *ids = new int[nPicks];
			int minDep = *(++ptr);
			int minIndex = 0;
			ptr--;
			for(int j = 0; j < nPicks; j++)
			{
				objID = *ptr;
				ptr++;
				if(*ptr < minDep)
				{
					minDep = *ptr;
					minIndex = j;
				}
				ptr++;
				ptr++;

				for (int k = 0; k < objID; k++)
				{
					ptr++;
				}
				ptr--;
				selectedID = *ptr;
				ids[j] = selectedID;
				//std::cout << selectedID << std::endl;
				if(selectedID >= 400) //HACK ULAN
					minIndex = j;
				ptr++;
			}
			selectedID = ids[minIndex];
			//std::cout << selectedID << std::endl;
			GUI::selectedID = selectedID;
			GUI::absorbeEvents = true;
			Input::resetButtons();
			}
		else
		{
			GUI::absorbeEvents = false;
			GUI::selectedID = -1;
		}
}

int GUI::selectedID = -1;
int GUI::lastGivenID = 0;
GLuint GUI::pickBuffer[4096];

bool GUI::absorbeEvents = false;
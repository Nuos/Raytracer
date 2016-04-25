#include <iostream>
#include <math.h>
#include <glut.h>

#include "Input.h"

#ifndef _GUI_H
#define _GUI_H

class GUI
{
public:
	GUI();
	static void end();
	static void initializeGUI(bool select);
	static void processPicks();
	static bool Button( int x, int y, int width, int height, char *string );
	static bool ArrowRight(int x, int y, int size);
	static bool ArrowLeft(int x, int y, int size);
	static void Label(int x, int y, char *string);
	static bool ClickableLine(int x1, int y1, int x2, int y2);

	static int selectedID;
	static int lastGivenID;
	static GLuint pickBuffer[4096];

	static bool absorbeEvents;
};

#endif
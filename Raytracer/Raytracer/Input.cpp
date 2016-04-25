#include "Input.h"
#include <iostream>
#include <glut.h>
#include "GUI.h"

bool Input::getMouseButton(int button)
{
	return ( Input::buttonStates[button] == PRESSED );
}

bool Input::getMouseButtonDown(int button)
{
	return ( Input::buttonStates[button] == DOWN );
}

bool Input::getMouseButtonUp(int button)
{
	return ( Input::buttonStates[button] == UP );
}

void Input::onMouseFunction(int button, int state, int x, int y)
{
	switch( state )
	{
		case GLUT_DOWN :
		{
			if( button == 0 )
			{
				Input::buttonStates[0] = DOWN;
			}
			else if( button == 1 )
			{
				Input::buttonStates[1] = DOWN;
			}
			else if( button == 2 )
			{
				Input::buttonStates[2] = DOWN;
			} break;
		}
		case GLUT_UP :
		{
			if( button == 0 )
			{
				Input::buttonStates[0] = UP;
			}
			else if( button == 1 )
			{
				Input::buttonStates[1] = UP;
			}
			else if( button == 2 )
			{
				Input::buttonStates[2] = UP;
			} break;	
		}
	}

	Input::mousePosition.x = x;
	Input::mousePosition.y = y;
}

void Input::onMotionFunction(int x, int y)
{
	Input::mousePosition.x = x;
	Input::mousePosition.y = y;
}

void Input::onPassiveMotionFunction(int x, int y)
{
	Input::mousePosition.x = x;
	Input::mousePosition.y = y;
}

void Input::update()
{
	//GUI::absorbeEvents = false;

	for( int i = 0; i < 3; i++ )
	{
		if ( Input::buttonStates[i] == DOWN )
			Input::buttonStates[i] = PRESSED;
		else if ( Input::buttonStates[i] == UP )
			Input::buttonStates[i] = RELEASED;
	}

	previousMousePosition = Input::mousePosition;
}

void Input::resetButtons()
{
	for( int i = 0; i < 3; i++ )
		Input::buttonStates[i] = RELEASED;
}

Vector3 Input::mousePosition = *new Vector3();
Vector3 Input::previousMousePosition = *new Vector3();
Input::ButtonState Input::buttonStates[3];
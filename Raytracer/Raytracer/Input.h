#ifndef _INPUT_H
#define _INPUT_H

#include "Vector3.h"

class Input
{
public:
	enum ButtonState
	{
		RELEASED,
		DOWN,
		PRESSED,
		UP
	};

	static Vector3 mousePosition;
	static Vector3 previousMousePosition;

	static bool getMouseButton(int button);
	static bool getMouseButtonDown(int button);
	static bool getMouseButtonUp(int button);

	static void onMouseFunction(int button, int state, int x, int y);
	static void onMotionFunction(int x, int y);
	static void onPassiveMotionFunction(int x, int y);

	static void update();
	static void resetButtons();

	static ButtonState buttonStates[3];
};

#endif
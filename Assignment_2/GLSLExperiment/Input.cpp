#include <iostream>
#include "Input.h"

using namespace assignment2;

static auto DoNothingFunc = []() {};

char Input::prevKey = '\0';
char Input::currentKey = '\0';
std::map<char, Input::KbFunc> Input::kbFuncsMapper = std::map<char, KbFunc>();

void Input::KbEventHandler(unsigned char key, int x, int y)
{
	Input::currentKey = key;
	if (kbFuncsMapper.find(key) != kbFuncsMapper.end())
	{
		auto func = kbFuncsMapper[key];
		func();
	}
	if (key == 'W')
	{
		Input::prevKey = '\0';
		Input::currentKey = '\0';
	}
	else
	{
		Input::prevKey = key;
	}

	if (key == 033) exit(EXIT_SUCCESS);
}

void Input::InitKbFuncs()
{
	kbFuncsMapper['W'] = []() {
		MeshPainter * pPainter = MeshPainter::CurrentDrawingInstance();
		if (pPainter != nullptr)
		{
			pPainter->rigid = RigidBodyMov();
			pPainter->deform = Deform();
			glutPostRedisplay();
			glutIdleFunc(DoNothingFunc);
		}
	};

	kbFuncsMapper['X'] = Input::DoTranslation;
	kbFuncsMapper['x'] = Input::DoTranslation;
	kbFuncsMapper['H'] = Input::DoShear;
	kbFuncsMapper['h'] = Input::DoShear;

	Input::prevKey = '\0';
	Input::currentKey = '\0';
}

void Input::DoTranslation()
{
	const float MOV_INCR = 0.0003f;
	const int STILL = 0;
	const int FORWARD = 1;
	const int BACKWARD = -1;

	static float mov = 0.0f;
	static int state = STILL;
	MeshPainter * pPainter = MeshPainter::CurrentDrawingInstance();
	if (pPainter != nullptr)
	{
		if (Input::prevKey == Input::currentKey &&
			state != STILL)
		{
			mov = 0.0f;
			glutIdleFunc(DoNothingFunc);
			state = STILL;
		}
		else if (Input::currentKey == 'X')
		{
			state = FORWARD;
			mov = (float)state * MOV_INCR;
		}
		else if (Input::currentKey == 'x')
		{
			state = BACKWARD;
			mov = (float)state * MOV_INCR;
		}
		else return;

		auto idleMov = []() {
			MeshPainter * pPainter = MeshPainter::CurrentDrawingInstance();
			if (pPainter != nullptr)
			{
				pPainter->rigid.translate(mov, 0.0f, 0.0f);
				glutPostRedisplay();
			}
		};
		glutIdleFunc(idleMov);
		pPainter->rigid.translate(mov, 0.0f, 0.0f);
		glutPostRedisplay();
	}
}

void Input::DoShear()
{
	const float SHEAR = 0.02f;
	const int STILL = 0;
	const int INCR = 1;
	const int DECR = -1;

	float val = 0.0f;
	static int state = STILL;
	MeshPainter * pPainter = MeshPainter::CurrentDrawingInstance();
	if (pPainter != nullptr)
	{
		if (Input::prevKey == Input::currentKey &&
			state != STILL)
		{
			val = 0.0f;
			state = STILL;
		}
		else if (Input::currentKey == 'h')
		{
			state = INCR;
		}
		else if (Input::currentKey == 'H')
		{
			state = DECR;
		}
		else return;

		val = (float)state * SHEAR;

		Deform::ShearVal shear;
		shear.first = Deform::X_AXIS;
		shear.second = val;
		pPainter->deform.addShear(shear);
		glutPostRedisplay();
	}
}
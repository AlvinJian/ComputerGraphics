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
			glutPostRedisplay();
			glutIdleFunc(DoNothingFunc);
		}
	};

	kbFuncsMapper['X'] = Input::DoTranslation;
	kbFuncsMapper['x'] = Input::DoTranslation;

	Input::prevKey = '\0';
	Input::currentKey = '\0';
}

void Input::DoTranslation()
{
#define MOV_INCR 0.0002f
#define STILL 0
#define FORWARD 1
#define BACKWARD -1
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
			mov = MOV_INCR;
			state = FORWARD;
		}
		else if (Input::currentKey == 'x')
		{
			mov = -MOV_INCR;
			state = BACKWARD;
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
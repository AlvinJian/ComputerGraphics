#include <iostream>
#include "Input.h"
#include "Gallery.h"

using namespace assignment2;

static void DoNothingFunc()
{
	/* std::cout << __FUNCTION__ << std::endl;
	auto* pPainter = MeshPainter::CurrentDrawingInstance();
	pPainter->rigid.rotate(0.0f, 0.05f, 0.0f);
	glutPostRedisplay(); */
}

char Input::prevKey = '\0';
char Input::currentKey = '\0';
std::map<char, Input::KbFunc> Input::kbFuncsMapper = std::map<char, KbFunc>();
Gallery * Input::gallery = nullptr;

void Input::SetGallery(Gallery * g)
{
	gallery = g;
}

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
	kbFuncsMapper['T'] = Input::DoTwist;
	kbFuncsMapper['t'] = Input::DoTwist;
	kbFuncsMapper['N'] = Input::IteratePly;
	kbFuncsMapper['P'] = Input::IteratePly;
	kbFuncsMapper['c'] = Input::TogglePalette;

	Input::prevKey = '\0';
	Input::currentKey = '\0';
}

void Input::DoTranslation()
{
	// TODO make it 3D translation
	const float MOV_INCR = 0.0003f;
	const int STILL = 0;
	const int FORWARD = 1;
	const int BACKWARD = -1;

	static Angel::vec3 mov;
	static int state = STILL;
	MeshPainter * pPainter = MeshPainter::CurrentDrawingInstance();
	if (pPainter != nullptr)
	{
		mov = Angel::vec3(0.0f, 0.0f, 0.0f);
		if (Input::prevKey == Input::currentKey &&
			state != STILL)
		{
			glutIdleFunc(DoNothingFunc);
			state = STILL;
		}
		else if (Input::currentKey == 'X')
		{
			state = FORWARD;
			mov.x += (float)state * MOV_INCR;
		}
		else if (Input::currentKey == 'x')
		{
			state = BACKWARD;
			mov.x += (float)state * MOV_INCR;
		}
		else return;

		auto idleMov = []() {
			MeshPainter * pPainter = MeshPainter::CurrentDrawingInstance();
			if (pPainter != nullptr)
			{
				pPainter->rigid.translate(mov.x, mov.y, mov.z);
				glutPostRedisplay();
			}
		};
		glutIdleFunc(idleMov);
		pPainter->rigid.translate(mov.x, mov.y, mov.z);
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
		if (Input::currentKey == 'h')
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

void Input::DoTwist()
{
	const float TWIST = 30.0f;
	const int STILL = 0;
	const int INCR = 1;
	const int DECR = -1;

	float val = 0.0f;
	static int state = STILL;
	MeshPainter * pPainter = MeshPainter::CurrentDrawingInstance();
	if (pPainter != nullptr)
	{
		if (Input::currentKey == 't')
		{
			state = INCR;
		}
		else if (Input::currentKey == 'T')
		{
			state = DECR;
		}
		else return;

		val = (float)state * TWIST;
		pPainter->deform.addYTwist(val);
		glutPostRedisplay();
	}
}

void Input::IteratePly()
{
	MeshPainter * instance = MeshPainter::CurrentDrawingInstance();
	if (gallery == nullptr || instance == nullptr) 
		return;
	if (Input::currentKey == 'N')
	{
		Ply & p = gallery->next();
		instance->draw(p);
		glutPostRedisplay();
	}
	else if (Input::currentKey == 'P')
	{
		Ply & p = gallery->prev();
		instance->draw(p);
		glutPostRedisplay();
	}
	else return;
}

void Input::TogglePalette()
{
	// ugly stuff
	const int RED_PALLETE = 0;
	const std::vector<color4> ColorPalette{
		color4(1.0, 0.0, 0.0, 1.0),  // red
		color4(0.0, 0.0, 0.0, 1.0),  // black
		color4(1.0, 1.0, 0.0, 1.0),  // yellow
		color4(0.0, 1.0, 0.0, 1.0),  // green
		color4(0.0, 0.0, 1.0, 1.0),  // blue
		color4(1.0, 0.0, 1.0, 1.0),  // magenta
		color4(1.0, 1.0, 1.0, 1.0),  // white
		color4(0.0, 1.0, 1.0, 1.0)   // cyan
	};
	const int COLOR_PALETTE = 1;
	const std::vector<color4> RedPalette{
		color4(1.0, 0.0, 0.0, 1.0),  // red
	};
	static int PaletteState = RED_PALLETE;
	MeshPainter * pPainter = MeshPainter::CurrentDrawingInstance();
	if (PaletteState == RED_PALLETE)
	{
		PaletteState = COLOR_PALETTE;
		pPainter->setPalette(ColorPalette);
	}
	else if (PaletteState == COLOR_PALETTE)
	{
		PaletteState = RED_PALLETE;
		pPainter->setPalette(RedPalette);
	}
	pPainter->updateDraw();
	glutPostRedisplay();
}
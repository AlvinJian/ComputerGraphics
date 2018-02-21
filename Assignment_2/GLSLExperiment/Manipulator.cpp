#include <iostream>
#include "Manipulator.h"
#include "Gallery.h"

using namespace assignment2;

static void DoNothingFunc()
{
}

char Manipulator::prevKey = '\0';
char Manipulator::currentKey = '\0';
std::map<char, Manipulator::KbFunc> Manipulator::kbFuncsMapper = std::map<char, KbFunc>();
Gallery * Manipulator::gallery = nullptr;

int Manipulator::TranslateState = 0;
int Manipulator::ShearState = 0;
int Manipulator::TwistState = 0;
int Manipulator::PaletteState = 0;
int Manipulator::SelfRotState = 0;

void Manipulator::SetGallery(Gallery * g)
{
	gallery = g;
}

void Manipulator::KbEventHandler(unsigned char key, int x, int y)
{
	Manipulator::currentKey = key;
	if (kbFuncsMapper.find(key) != kbFuncsMapper.end())
	{
		auto func = kbFuncsMapper[key];
		func();
	}
	if (key == 'W')
	{
		Manipulator::prevKey = '\0';
		Manipulator::currentKey = '\0';
	}
	else
	{
		Manipulator::prevKey = key;
	}

	if (key == 033) exit(EXIT_SUCCESS);
}

void Manipulator::InitKbFuncs()
{
	kbFuncsMapper['W'] = []() {
		MeshPainter * pPainter = MeshPainter::CurrentDrawingInstance();
		if (pPainter != nullptr)
		{
			TranslateState = 0;
			ShearState = 0;
			TwistState = 0;
			PaletteState = 0;
			SelfRotState = 0;

			pPainter->rigid = RigidBodyMov();
			pPainter->deform = Deform();
			glutPostRedisplay();
			glutIdleFunc(DoNothingFunc);
		}
	};

	kbFuncsMapper['X'] = Manipulator::DoTranslation;
	kbFuncsMapper['x'] = Manipulator::DoTranslation;
	kbFuncsMapper['H'] = Manipulator::DoShear;
	kbFuncsMapper['h'] = Manipulator::DoShear;
	kbFuncsMapper['T'] = Manipulator::DoTwist;
	kbFuncsMapper['t'] = Manipulator::DoTwist;
	kbFuncsMapper['N'] = Manipulator::IteratePly;
	kbFuncsMapper['P'] = Manipulator::IteratePly;
	kbFuncsMapper['c'] = Manipulator::TogglePalette;
	kbFuncsMapper['R'] = Manipulator::DoSelfRotate;

	Manipulator::prevKey = '\0';
	Manipulator::currentKey = '\0';
}

void Manipulator::DoTranslation()
{
	// TODO make it 3D translation
	const float MOV_INCR = 0.0003f;
	const int STILL = 0;
	const int FORWARD = 1;
	const int BACKWARD = -1;

	static Angel::vec3 mov;
	MeshPainter * pPainter = MeshPainter::CurrentDrawingInstance();
	if (pPainter != nullptr)
	{
		mov = Angel::vec3(0.0f, 0.0f, 0.0f);
		if (Manipulator::prevKey == Manipulator::currentKey &&
			TranslateState != STILL)
		{
			glutIdleFunc(DoNothingFunc);
			TranslateState = STILL;
		}
		else if (Manipulator::currentKey == 'X')
		{
			TranslateState = FORWARD;
			mov.x += (float)TranslateState * MOV_INCR;
		}
		else if (Manipulator::currentKey == 'x')
		{
			TranslateState = BACKWARD;
			mov.x += (float)TranslateState * MOV_INCR;
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

void Manipulator::DoShear()
{
	const float SHEAR = 0.02f;
	const int STILL = 0;
	const int INCR = 1;
	const int DECR = -1;

	float val = 0.0f;
	MeshPainter * pPainter = MeshPainter::CurrentDrawingInstance();
	if (pPainter != nullptr)
	{
		if (Manipulator::currentKey == 'h')
		{
			ShearState = INCR;
		}
		else if (Manipulator::currentKey == 'H')
		{
			ShearState = DECR;
		}
		else return;

		val = (float)ShearState * SHEAR;

		Deform::ShearVal shear;
		shear.first = Deform::X_AXIS;
		shear.second = val;
		pPainter->deform.addShear(shear);
		glutPostRedisplay();
	}
}

void Manipulator::DoTwist()
{
	const float TWIST = 30.0f;
	const int STILL = 0;
	const int INCR = 1;
	const int DECR = -1;

	float val = 0.0f;
	MeshPainter * pPainter = MeshPainter::CurrentDrawingInstance();
	if (pPainter != nullptr)
	{
		if (Manipulator::currentKey == 't')
		{
			TwistState = INCR;
		}
		else if (Manipulator::currentKey == 'T')
		{
			TwistState = DECR;
		}
		else return;

		val = (float)TwistState * TWIST;
		pPainter->deform.addYTwist(val);
		glutPostRedisplay();
	}
}

void Manipulator::IteratePly()
{
	MeshPainter * instance = MeshPainter::CurrentDrawingInstance();
	if (gallery == nullptr || instance == nullptr) 
		return;
	if (Manipulator::currentKey == 'N')
	{
		Ply & p = gallery->next();
		instance->draw(p);
		glutPostRedisplay();
	}
	else if (Manipulator::currentKey == 'P')
	{
		Ply & p = gallery->prev();
		instance->draw(p);
		glutPostRedisplay();
	}
	else return;
}

void Manipulator::TogglePalette()
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

void Manipulator::DoSelfRotate()
{
	auto* pPainter = MeshPainter::CurrentDrawingInstance();
	if (pPainter == nullptr) return;

	static const int STILL = 0;
	static const int CLOCK = -1;
	static const int CCLOCK = 1;

	static const float DEG_INC = 0.05f;
	static float degree = 0.0f;

	auto idleFunc = []() {
		auto* pPainter = MeshPainter::CurrentDrawingInstance();
		if (pPainter == nullptr) return;
		Angel::vec3& rot = pPainter->rigid.getRotate();
		if (std::abs(rot.y) >= 360.0f)
		{
			std::cout << "next model" << std::endl;
			rot.y = 0.0f;
			degree *= -1.0f;
			Ply & p = gallery->next();
			pPainter->draw(p);
			glutPostRedisplay();
			return;
		}
		if (degree > 0.0f)
		{
			SelfRotState = CCLOCK;
		}
		else if (degree < 0.0f)
		{
			SelfRotState = CLOCK;
		}
		pPainter->rigid.rotate(0.0f, degree, 0.0f);
		glutPostRedisplay();
	};

	if (SelfRotState == STILL)
	{
		SelfRotState = CLOCK;
		degree = (float)SelfRotState * DEG_INC;
		idleFunc();
		glutIdleFunc(idleFunc);
		glutPostRedisplay();
	}
}
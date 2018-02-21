#include <iostream>
#include "Manipulator.h"
#include "Gallery.h"

using namespace assignment2;

static auto _DoNothing = []() {};
#define REG_NUM 4
#define TRANS_REG 0
#define ROT_REG 1
std::vector<Manipulator::IdleFunc> Manipulator::funcs = 
	std::vector<Manipulator::IdleFunc>(REG_NUM, _DoNothing );
void Manipulator::RegisterIdleFunctions(size_t reg, IdleFunc f)
{
	if (reg < REG_NUM)
	{
		funcs[reg] = f;
	}
}
void Manipulator::RunIdleFunctions()
{
	for (size_t i = 0; i < REG_NUM; ++i)
	{
		funcs[i]();
	}
}

char Manipulator::prevKey = '\0';
char Manipulator::currentKey = '\0';
std::map<char, Manipulator::KbFunc> Manipulator::kbFuncsMapper = std::map<char, KbFunc>();
Gallery * Manipulator::gallery = nullptr;

int Manipulator::TranslateState = 0;
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

void Manipulator::Reset()
{
	MeshPainter * pPainter = MeshPainter::CurrentDrawingInstance();
	if (pPainter != nullptr)
	{
		pPainter->rigid = RigidBodyMov();
		pPainter->deform = Deform();
		for (size_t i = 0; i < REG_NUM; ++i)
		{
			Manipulator::RegisterIdleFunctions(i, _DoNothing);
		}
		TranslateState = 0;
		SelfRotState = 0;
		glutPostRedisplay();
	}
}

void Manipulator::Init()
{
	kbFuncsMapper['W'] = Manipulator::Reset;

	kbFuncsMapper['X'] = Manipulator::DoTranslation;
	kbFuncsMapper['x'] = Manipulator::DoTranslation;
	kbFuncsMapper['Y'] = Manipulator::DoTranslation;
	kbFuncsMapper['y'] = Manipulator::DoTranslation;
	kbFuncsMapper['Z'] = Manipulator::DoTranslation;
	kbFuncsMapper['z'] = Manipulator::DoTranslation;

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

	glutIdleFunc(RunIdleFunctions);
}

void Manipulator::DoTranslation()
{
	// TODO make it 3D translation
	const float MOV_INCR = 0.0003f;
	const int STILL = 0;
	const int FORWARD = 1;
	const int BACKWARD = -1;

	// static int TranslateState = STILL;
	Angel::vec3 mov(0.0f, 0.0f, 0.0f);
	MeshPainter * pPainter = MeshPainter::CurrentDrawingInstance();
	if (pPainter != nullptr)
	{
		if (Manipulator::prevKey == Manipulator::currentKey &&
			TranslateState != STILL)
		{
			Manipulator::RegisterIdleFunctions(TRANS_REG, _DoNothing);
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
		else if (Manipulator::currentKey == 'Y')
		{
			TranslateState = FORWARD;
			mov.y += (float)TranslateState * MOV_INCR;
		}
		else if (Manipulator::currentKey == 'y')
		{
			TranslateState = BACKWARD;
			mov.y += (float)TranslateState * MOV_INCR;
		}
		else if (Manipulator::currentKey == 'Z')
		{
			TranslateState = FORWARD;
			mov.z += (float)TranslateState * MOV_INCR;
		}
		else if (Manipulator::currentKey == 'z')
		{
			TranslateState = BACKWARD;
			mov.z += (float)TranslateState * MOV_INCR;
		}
		else return;

		auto idleMov = [mov]() {
			MeshPainter * pPainter = MeshPainter::CurrentDrawingInstance();
			if (pPainter != nullptr)
			{
				pPainter->rigid.translate(mov.x, mov.y, mov.z);
				glutPostRedisplay();
			}
		};
		
		RegisterIdleFunctions(TRANS_REG, idleMov);
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

	int ShearState = STILL;

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
	int TwistState = STILL;
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
	MeshPainter * pPainter = MeshPainter::CurrentDrawingInstance();
	if (gallery == nullptr || pPainter == nullptr) 
		return;
	// pPainter->rigid = RigidBodyMov();
	// pPainter->deform = Deform();
	SelfRotState = 0;
	Manipulator::RegisterIdleFunctions(ROT_REG, _DoNothing);
	if (Manipulator::currentKey == 'N')
	{
		Ply & p = gallery->next();
		pPainter->draw(p);
		glutPostRedisplay();
	}
	else if (Manipulator::currentKey == 'P')
	{
		Ply & p = gallery->prev();
		pPainter->draw(p);
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

void Manipulator::DoSelfRotate()
{
	auto* pPainter = MeshPainter::CurrentDrawingInstance();
	if (pPainter == nullptr) return;

	static const int STILL = 0;
	static const int CLOCK = -1;
	static const int CCLOCK = 1;
	static const float DEG_INC = 0.05f;

	static float degCumulator = 0.0f;

	auto idleFunc = []() {
		static float degree = 0.0f;
		auto* pPainter = MeshPainter::CurrentDrawingInstance();
		if (pPainter == nullptr) return;
		Angel::vec3& rot = pPainter->rigid.getRotate();
		if (degCumulator >= 358.0f)
		{
			rot.y = 0.0f;
			degCumulator = 0.0f;
			SelfRotState *= -1;
			rot.y = 0.0f;
			std::cout << "SelfRotState=" << SelfRotState << std::endl;
			Ply & p = gallery->next();
			pPainter->draw(p);
			glutPostRedisplay();
		}
		else
		{
			degree = SelfRotState * DEG_INC;
			pPainter->rigid.rotate(0.0f, degree, 0.0f);
			degCumulator += std::fabsf(degree);
			glutPostRedisplay();
		}
	};

	if (SelfRotState == STILL)
	{
		degCumulator = 0.0f;
		SelfRotState = CCLOCK;
		idleFunc();
		Manipulator::RegisterIdleFunctions(ROT_REG, idleFunc);
		std::cout << "SelfRotState=" << SelfRotState << std::endl;
	}
	else
	{
		degCumulator = 0.0f;
		SelfRotState *= -1;
		std::cout << "SelfRotState=" << SelfRotState << std::endl;
	}
}
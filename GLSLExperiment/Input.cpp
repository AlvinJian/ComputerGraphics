#include "Input.h"
#include "Angel.h"
#include "SierpinskiGasket.h"
#include "GRS.h"
#include "Config.h"
#include "KochSnowflake.h"

using namespace assignment1;

void Input::KbEventHandler(unsigned char key, int x, int y)
{
	static int kochIter = DEFAULT_KOCH_ITER;
	switch (key) {
	case 's':
	case 'S':
	{
		SierpinskiGasket::Draw();
	}
	break;
	
	case 'k':
	case 'K':
	{
		kochIter = DEFAULT_KOCH_ITER;
		KochSnowflake::Draw(kochIter);
	}
	break;
	
	case 'i':
	case 'I':
	{
		++kochIter;
		if (kochIter >= 6)
		{
			kochIter = 6;
		}
		KochSnowflake::Draw(kochIter);
	}
	break;
	
	case 'r':
	case 'R':
	{
		--kochIter;
		if (kochIter < 1)
		{
			kochIter = 1;
		}
		KochSnowflake::Draw(kochIter);
	}
	break;
	case 'u':
	case 'U':
	{
		std::string fileName = "usa.dat";
		GRSData::Draw(fileName);
	}
	break;
	
	case 'd':
	case 'D':
	{
		std::string fileName = "dragon.dat";
		GRSData::Draw(fileName);
	}
	break;

	case 'v':
	case 'V':
	{
		std::string fileName = "vinci.dat";
		GRSData::Draw(fileName);
	}
	break;

	case 033:			// 033 is Escape key octal value
		exit(1);		// quit program
		break;
	}
	glutPostRedisplay();
}

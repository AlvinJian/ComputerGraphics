#include <string>
#include <cstdlib>
#include <ctime>
#include "PlyFile.h"
#include "MeshPainter.h"
#include "Input.h"
#include "Config.h"

#if 1
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(WINDOW_POS_X, WINDOW_POS_Y);
	glutCreateWindow("CS543 Computer Graphics");

	glewInit();
	std::srand(std::time(nullptr));
	config::ViewportConfig::Init();
	assignment2::Input::InitKbFuncs();

	std::string airplane("ply_files/apple.ply");
	assignment2::Ply * plyModel = assignment2::Ply::Load(airplane);
	std::vector<assignment2::color4> palette{
		assignment2::color4(1.0, 0.0, 0.0, 1.0),  // red
		assignment2::color4(0.0, 0.0, 0.0, 1.0),  // black
		assignment2::color4(1.0, 1.0, 0.0, 1.0),  // yellow
		assignment2::color4(0.0, 1.0, 0.0, 1.0),  // green
		assignment2::color4(0.0, 0.0, 1.0, 1.0),  // blue
		assignment2::color4(1.0, 0.0, 1.0, 1.0),  // magenta
		assignment2::color4(1.0, 1.0, 1.0, 1.0),  // white
		assignment2::color4(0.0, 1.0, 1.0, 1.0)   // cyan
	};
	assignment2::MeshPainter painter(palette);
	painter.draw(*plyModel);
	glutKeyboardFunc(assignment2::Input::KbEventHandler);
	glutPostRedisplay();
	glutMainLoop();
	return 0;
}
#endif
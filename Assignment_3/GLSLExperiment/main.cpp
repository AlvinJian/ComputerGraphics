#include <string>
#include <cstdlib>
#include <ctime>
#include "PlyFile.h"
#include "MeshPainter.h"
#include "Manipulator.h"
#include "Config.h"
#include "Gallery.h"

#if 1
using namespace assignment3;
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
	assignment3::Manipulator::Init();

	std::vector<color4> RedPalette{
		color4(1.0, 0.0, 0.0, 1.0),  // red
	};
	assignment3::MeshPainter painter(RedPalette);
	assignment3::Gallery gallery;
	assignment3::Manipulator::SetGallery(&gallery);
	painter.draw(gallery.current());
	glutKeyboardFunc(assignment3::Manipulator::KbEventHandler);
	glutPostRedisplay();
	glutMainLoop();
	return 0;
}
#endif
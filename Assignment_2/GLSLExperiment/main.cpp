#include <string>
#include <cstdlib>
#include <ctime>
#include "PlyFile.h"
#include "MeshPainter.h"
#include "Input.h"
#include "Config.h"
#include "Gallery.h"

#if 1
using namespace assignment2;
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

	std::vector<color4> RedPalette{
		color4(1.0, 0.0, 0.0, 1.0),  // red
	};
	assignment2::MeshPainter painter(RedPalette);
	assignment2::Gallery gallery;
	assignment2::Input::SetGallery(&gallery);
	painter.draw(gallery.current());
	glutKeyboardFunc(assignment2::Input::KbEventHandler);
	glutPostRedisplay();
	glutMainLoop();
	return 0;
}
#endif
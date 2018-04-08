#include <string>
#include <cstdlib>
#include <iostream>

#include "ImageRenderer.h"
#include "Config.h"
#include "Angel.h"

using namespace image;
using namespace utils;
using namespace config;

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(WINDOW_POS_X, WINDOW_POS_Y);
	glutCreateWindow("CS543 Computer Graphics");
	glewInit();

	ViewportConfig::Init();

	SingleImageRender imageRenderer;
	imageRenderer.use();
	std::string imageFilename("usain_bolt.bmp");
	imageRenderer.loadImageToTexture(imageFilename);

	auto reshape = [](int w, int h)
	{
		ViewportConfig::SetSize(w, h);
	};
	glutReshapeFunc(reshape);
	glutDisplayFunc(SingleImageRender::Render);
	glutPostRedisplay();

	glutMainLoop();

	return 0;
}
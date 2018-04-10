#include <string>
#include <cstdlib>
#include <iostream>

#include "ImageRenderer.h"
#include "Config.h"
#include "Manipulator.h"
#include "Angel.h"

using namespace image;
using namespace utils;
using namespace config;
using namespace ctrl;

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

	Manipulator manip;
	manip.use();

	ctrl::KbEventHandler resetCtrl = [](unsigned char k, int x, int y)
	{
		ImageRenderer * pRender = SingleImageRender::GetCurrent();
		if (pRender != nullptr)
		{
			std::cout << "reset" << std::endl;
			pRender->resetPostProcessShader();
			glutPostRedisplay();
		}
	};
	manip.addKeyFunc('O', resetCtrl);

	GLuint luminEffect = InitShader("vshader1.glsl", "lumin_fs.glsl");
	ctrl::KbEventHandler luminCtrl = [luminEffect](unsigned char k, int x, int y)
	{
		ImageRenderer * pRender = SingleImageRender::GetCurrent();
		if (pRender != nullptr)
		{
			std::cout << "luminProg=" << luminEffect << std::endl;
			pRender->setPostProcessShader(luminEffect);
			glutPostRedisplay();
		}
	};
	manip.addKeyFunc('L', luminCtrl);
	glutKeyboardFunc(Manipulator::KbEventCallback);

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
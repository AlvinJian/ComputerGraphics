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

	GLuint negativeEffect = InitShader("vshader1.glsl", "negative_fs.glsl");
	ctrl::KbEventHandler negativeCtrl = [negativeEffect](unsigned char k, int x, int y)
	{
		ImageRenderer * pRender = SingleImageRender::GetCurrent();
		if (pRender != nullptr)
		{
			std::cout << "negativeProg=" << negativeEffect << std::endl;
			pRender->setPostProcessShader(negativeEffect);
			glutPostRedisplay();
		}
	};
	manip.addKeyFunc('N', negativeCtrl);

	GLuint edgeDetectEffect = InitShader("vshader1.glsl", "edge_detect_fs.glsl");
	ctrl::KbEventHandler edgeDetectCtrl = [edgeDetectEffect](unsigned char k, int x, int y)
	{
		ImageRenderer * pRender = SingleImageRender::GetCurrent();
		if (pRender != nullptr)
		{
			std::cout << "edgeDetectProg=" << edgeDetectEffect << std::endl;
			pRender->setPostProcessShader(edgeDetectEffect);
			glutPostRedisplay();
		}
	};
	manip.addKeyFunc('D', edgeDetectCtrl);

	GLuint sphereEffect = InitShader("vshader1.glsl", "sphere_fs.glsl");
	ctrl::KbEventHandler sphereCtrl = [sphereEffect](unsigned char k, int x, int y)
	{
		ImageRenderer * pRender = SingleImageRender::GetCurrent();
		if (pRender != nullptr)
		{
			std::cout << "sphereProg=" << sphereEffect << std::endl;
			pRender->setPostProcessShader(sphereEffect);
			glutPostRedisplay();
		}
	};
	manip.addKeyFunc('S', sphereCtrl);

	GLuint embossEffect = InitShader("vshader1.glsl", "emboss_fs.glsl");
	ctrl::KbEventHandler embossCtrl = [embossEffect](unsigned char k, int x, int y)
	{
		ImageRenderer * pRender = SingleImageRender::GetCurrent();
		if (pRender != nullptr)
		{
			std::cout << "embossProg=" << embossEffect << std::endl;
			pRender->setPostProcessShader(embossEffect);
			glutPostRedisplay();
		}
	};
	manip.addKeyFunc('E', embossCtrl);

	GLuint toonEffect = InitShader("vshader1.glsl", "toon_fs.glsl");
	ctrl::KbEventHandler toonCtrl = [toonEffect](unsigned char k, int x, int y)
	{
		ImageRenderer * pRender = SingleImageRender::GetCurrent();
		if (pRender != nullptr)
		{
			std::cout << "toonProg=" << toonEffect << std::endl;
			pRender->setPostProcessShader(toonEffect);
			glutPostRedisplay();
		}
	};
	manip.addKeyFunc('T', toonCtrl);

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
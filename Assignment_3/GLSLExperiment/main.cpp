#include <string>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "PlyFile.h"
#include "MeshPainter.h"
#include "Manipulator.h"
#include "Config.h"
#include "Gallery.h"

#include "Scene.h"
#include "TransformNode.h"
#include "ModelNode.h"

using namespace assignment3;

int main(int argc, char* argv[])
{
#if 0
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
#endif
	std::cout.precision(3);
	std::cout << std::fixed;

	Scene scn;

	GroupNode root;
	ModelNode dummy;

	TransformNode toLeft(Angel::Translate(-0.3f, -0.3f, 0.0f));
	TransformNode toRight(Angel::Translate(0.3f, -0.3f, 0.0f));
	TransformNode toFront(Angel::Translate(0.0f, 0.0f, 0.3f));
	TransformNode toBack(Angel::Translate(0.0f, 0.0f, -0.3f));

	scn.setRoot(&root, Angel::vec3(0.0f, 1.0f, 0.0f));
	
	GroupNode grpLv1Left;
	GroupNode grpLv1Right;
	grpLv1Left.addChild(&toFront).addChild(&toLeft).addChild(&dummy);
	grpLv1Right.addChild(&toBack).addChild(&toRight).addChild(&dummy);
	root.addChild(&grpLv1Left).addChild(&grpLv1Right);

	GroupNode grpLv2LLeft;
	GroupNode grpLv2LRight;
	grpLv2LLeft.addChild(&toLeft).addChild(&dummy);
	grpLv2LRight.addChild(&toRight).addChild(&dummy);
	grpLv1Left.addChild(&grpLv2LLeft).addChild(&grpLv2LRight);

	scn.startRender();
	system("pause");
	return 0;
}
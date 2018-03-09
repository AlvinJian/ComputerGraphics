#include <string>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "PlyFile.h"
#include "Config.h"
#include "Gallery.h"

#include "Scene.h"
#include "TransformNode.h"
#include "AnimatorNode.h"
#include "Animation.h"
#include "ModelNode.h"
#include "ArmNode.h"

using namespace assignment3;

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(WINDOW_POS_X, WINDOW_POS_Y);
	glutCreateWindow("CS543 Computer Graphics");
	glewInit();

	config::ViewportConfig::Init();

	std::cout.precision(3);
	std::cout << std::fixed;

	assignment3::Gallery gallery;
	Scene scn;
	Scene::Use(&scn);

	GroupNode root;
	ModelNode dummy(gallery.current(), Angel::vec4(1.0, 0.0, 0.0, 1.0));
	dummy.setup();
	ModelNode dummy1(gallery.next(), Angel::vec4(0.0, 1.0, 0.0, 1.0));
	dummy1.setup();
	ModelNode dummy2(gallery.next(), Angel::vec4(0.0, 0.0, 1.0, 1.0));
	dummy2.setup();
	ModelNode dummy3(gallery.next(), Angel::vec4(1.0, 1.0, 0.0, 1.0));
	dummy3.setup();
	ModelNode dummy4(gallery.next(), Angel::vec4(1.0, 0.0, 1.0, 1.0));
	dummy4.setup();

	ArmNode toRight(Angel::vec3(-0.33f, -0.33f, 0.0f), TransformNode::RIGHT);
	ArmNode leftEnd(Angel::vec3(0.66f, 0.0f, 0.0f));

	AnimationEngine animEngine;
	AnimationEngine::Use(&animEngine);
	RotateAnimatorNode rotAnim(300, RotateAnimatorNode::Y_AXIS);
	RotateAnimatorNode rotAnimRever(450, RotateAnimatorNode::Y_AXIS);
	rotAnimRever.reverse();
	animEngine.registerAnimator(&rotAnim);
	animEngine.registerAnimator(&rotAnimRever);

	scn.setRoot(&root, Angel::vec3(0.0f, 0.65f, 0.0f));
	root.addChild(&dummy);

	GroupNode grpLv1;
	grpLv1.addChild(&rotAnimRever);
	root.addChild(&grpLv1);
	grpLv1.addChild(&toRight).addChild(&dummy1);
	grpLv1.addChild(&leftEnd).addChild(&dummy2);

	GroupNode grpLv2; grpLv2.addChild(&rotAnim);
	grpLv1.addChild(&grpLv2);
	grpLv2.addChild(&toRight).addChild(&dummy3);
	grpLv2.addChild(&leftEnd).addChild(&dummy4);

	auto reshape = [](int w, int h)
	{
		config::ViewportConfig::SetSize(w, h);
	};
	glutReshapeFunc(reshape);
	glutDisplayFunc(Scene::Render);
	glutIdleFunc(AnimationEngine::Playback);
	glutPostRedisplay();
	glutMainLoop();
	return 0;
}
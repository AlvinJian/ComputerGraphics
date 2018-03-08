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

	TransformNode toLeft(Angel::Translate(-0.5f, -0.35f, 0.0f), TransformNode::RIGHT);
	TransformNode toRight(Angel::Translate(0.5f, -0.35f, 0.0f), TransformNode::RIGHT);

	AnimationEngine animEngine;
	AnimationEngine::Use(&animEngine);
	RotateAnimatorNode rotAnim(300, RotateAnimatorNode::Y_AXIS);
	RotateAnimatorNode rotAnimRever(450, RotateAnimatorNode::Y_AXIS);
	rotAnimRever.reverse();
	animEngine.registerAnimator(&rotAnim);
	animEngine.registerAnimator(&rotAnimRever);

	scn.setRoot(&root, Angel::vec3(0.0f, 0.5f, 0.0f));
	root.addChild(&dummy);

	GroupNode grpLv1Left;
	GroupNode grpLv1Right;
	grpLv1Left.addChild(&toLeft).addChild(&dummy1);
	grpLv1Right.addChild(&toRight).addChild(&dummy2);
	root.addChild(&rotAnimRever).addChild(&grpLv1Left).addChild(&grpLv1Right);

	GroupNode grpLv2LLeft;
	GroupNode grpLv2LRight;
	grpLv2LLeft.addChild(&toLeft).addChild(&dummy3);
	grpLv2LRight.addChild(&toRight).addChild(&dummy4);
	
	grpLv1Right.addChild(&rotAnim).addChild(&grpLv2LLeft).addChild(&grpLv2LRight);

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
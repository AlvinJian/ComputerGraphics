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
#include "Manipulator.h"
#include "SinusoidAnimator.h"

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

	Gallery gallery;
	Scene scn;
	scn.use();

	ModelNode dummy(gallery.current(), Angel::vec4(1.0, 0.0, 0.0, 1.0)); // done
	dummy.setup();
	ModelNode dummy1(gallery.next(), Angel::vec4(0.0, 1.0, 0.0, 1.0)); // done
	dummy1.setup();
	ModelNode dummy2(gallery.next(), Angel::vec4(0.0, 0.0, 1.0, 1.0)); // done
	dummy2.setup();
	ModelNode dummy3(gallery.next(), Angel::vec4(1.0, 1.0, 0.0, 1.0)); // done
	dummy3.setup();
	ModelNode dummy4(gallery.next(), Angel::vec4(1.0, 0.0, 1.0, 1.0)); // done
	dummy4.setup();
	ModelNode dummy5(gallery.next(), Angel::vec4(0.0, 1.0, 1.0, 1.0)); // done
	dummy5.setup();
	ModelNode dummy6(gallery.next(), Angel::vec4(0.5, 0.55, 0.35, 1.0)); // done
	dummy6.setup();
	ModelNode dummy7(gallery.next(), Angel::vec4(0.3, 0.8, 0.5, 1.0)); // done
	dummy7.setup();

	ArmNode toRight(Angel::vec3(-0.33f, -0.33f, 0.0f), TransformNode::RIGHT);
	ArmNode toLeft(Angel::vec3(0.33f, -0.33f, 0.0f), TransformNode::RIGHT);
	ArmNode leftEnd(Angel::vec3(0.66f, 0.0f, 0.0f));

	AnimationEngine animEngine;
	animEngine.use();
	RotateAnimatorNode rotAnim(250, RotateAnimatorNode::Y_AXIS);
	RotateAnimatorNode rotAnimRever(400, RotateAnimatorNode::Y_AXIS);
	RotateAnimatorNode rotAnimRever2(150, RotateAnimatorNode::Y_AXIS);
	rotAnimRever.reverse();
	rotAnimRever2.reverse();
	animEngine.registerAnimator(&rotAnim);
	animEngine.registerAnimator(&rotAnimRever);
	animEngine.registerAnimator(&rotAnimRever2);

	SinusoidAnimator sinuAnim(500, TransformNode::Y_AXIS, 0.05f);
	animEngine.registerAnimator(&sinuAnim);
	// toRight.linkSinusoidAnimator(&sinuAnim);
	SinusoidAnimator sinuAnimOffset(500, TransformNode::Y_AXIS, 0.05f, 90.0f);
	animEngine.registerAnimator(&sinuAnimOffset);
	// toLeft.linkSinusoidAnimator(&sinuAnimOffset);
	// leftEnd.linkSinusoidAnimator(&sinuAnimOffset);

	GroupNode root;
	scn.setRoot(&root, Angel::vec3(0.0f, 0.65f, 0.0f));
	root.addChild(&dummy);

	GroupNode grpLv1; grpLv1.addChild(&rotAnimRever);
	root.addChild(&grpLv1);
	grpLv1.addChild(&toRight).addChild(&dummy1);
	grpLv1.addChild(&leftEnd).addChild(&dummy2);

	GroupNode grpLv2; grpLv2.addChild(&rotAnim);
	grpLv1.addChild(&grpLv2);
	grpLv2.addChild(&toRight).addChild(&dummy3);
	GroupNode grpLv3; grpLv3.addChild(&rotAnimRever2);
	grpLv2.addChild(&grpLv3);
	grpLv2.addChild(&leftEnd).addChild(&dummy4);

	grpLv3.addChild(&toRight).addChild(&dummy5);
	GroupNode grpLv4; grpLv4.addChild(&rotAnim);
	grpLv3.addChild(&grpLv4);
	grpLv3.addChild(&leftEnd).addChild(&dummy6);

	grpLv4.addChild(&toLeft).addChild(&dummy7);


	auto reshape = [](int w, int h)
	{
		config::ViewportConfig::SetSize(w, h);
	};
	glutReshapeFunc(reshape);
	glutDisplayFunc(Scene::Render);
	glutIdleFunc(AnimationEngine::Playback);
	glutPostRedisplay();

	Manipulator manip;
	manip.use();
	glutKeyboardFunc(Manipulator::KbEventCallback);

	glutMainLoop();
	return 0;
}
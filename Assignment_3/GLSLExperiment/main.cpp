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
	scn.LightPosition = Angel::vec4(-0.2f, 1.0f, 0.0f, 1.0f);
	scn.LightPositionEnd = Angel::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	scn.LightAngle = 45.0f;
	scn.Shininess = 6.0f;
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
	// ArmNode leftEnd(Angel::vec3(0.66f, 0.0f, 0.0f));

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

	SinusoidAnimator sinuAnim(500, TransformNode::Y_AXIS, 0.06f);
	animEngine.registerAnimator(&sinuAnim);
	toRight.linkSinusoidAnimator(&sinuAnim);
	SinusoidAnimator sinuAnimOffset(500, TransformNode::Y_AXIS, 0.06f, 180.0f);
	animEngine.registerAnimator(&sinuAnimOffset);
	toLeft.linkSinusoidAnimator(&sinuAnimOffset);
	// leftEnd.linkSinusoidAnimator(&sinuAnimOffset);

	GroupNode root;
	scn.setRoot(&root, Angel::vec3(0.0f, 0.65f, 0.0f));
	root.addChild(&dummy);

	GroupNode grpLv1R; grpLv1R.addChild(&rotAnimRever); root.addChild(&grpLv1R);
	grpLv1R.addChild(&toRight).addChild(&dummy1);
	GroupNode grpLv1L; grpLv1L.addChild(&rotAnimRever);
	root.addChild(&grpLv1L);
	grpLv1L.addChild(&toLeft).addChild(&dummy2);

	GroupNode grpLv2R; grpLv2R.addChild(&rotAnim); grpLv1L.addChild(&grpLv2R);
	grpLv2R.addChild(&toRight).addChild(&dummy3);
	GroupNode grpLv2L; grpLv2L.addChild(&rotAnim); grpLv1L.addChild(&grpLv2L);
	grpLv2L.addChild(&toLeft).addChild(&dummy4);

	GroupNode grpLv3R; grpLv3R.addChild(&rotAnimRever2); grpLv2L.addChild(&grpLv3R);
	grpLv3R.addChild(&toRight).addChild(&dummy5);
	GroupNode grpLv3L; grpLv3L.addChild(&rotAnimRever2); grpLv2L.addChild(&grpLv3L);
	grpLv3L.addChild(&toLeft).addChild(&dummy6);

	GroupNode grpLv4; grpLv4.addChild(&rotAnim);
	grpLv3R.addChild(&grpLv4);
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
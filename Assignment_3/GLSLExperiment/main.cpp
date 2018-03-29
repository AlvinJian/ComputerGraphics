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
#include "SkelBoxNode.h"

using namespace common;
using namespace anim;
using namespace scn;

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

	AnimationEngine animEngine;
	animEngine.use();
	RotateAnimatorNode selfRotAnim(80, RotateAnimatorNode::Y_AXIS);
	RotateAnimatorNode rotAnim(250, RotateAnimatorNode::Y_AXIS);
	RotateAnimatorNode rotAnimRever(400, RotateAnimatorNode::Y_AXIS);
	RotateAnimatorNode rotAnimRever2(150, RotateAnimatorNode::Y_AXIS);
	rotAnimRever.reverse();
	rotAnimRever2.reverse();
	animEngine.registerAnimator(rotAnim);
	animEngine.registerAnimator(rotAnimRever);
	animEngine.registerAnimator(rotAnimRever2);
	animEngine.registerAnimator(selfRotAnim);

	Gallery gallery;
	Scene mainScene;
	mainScene.LightPosition = Angel::vec4(-0.33f, 1.2f, 0.0f, 1.0f);
	mainScene.LightPositionEnd = Angel::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	mainScene.LightAngle = 60.0f;
	mainScene.Shininess = 8.0f;
	mainScene.use();

	SkelBoxNode box(Angel::vec4(0.0f, 0.5f, 0.3f, 1.0f));

	GroupNode dummy;
	ModelNode model(gallery.current(), Angel::vec4(1.0, 0.0, 0.0, 1.0)); // done
	model.setup();
	dummy.addChild(box).addChild(selfRotAnim).addChild(model);
	
	GroupNode dummy1;
	ModelNode model1(gallery.next(), Angel::vec4(0.0, 1.0, 0.0, 1.0)); // done
	model1.setup();
	dummy1.addChild(box).addChild(selfRotAnim).addChild(model1);

	GroupNode dummy2;
	ModelNode model2(gallery.next(), Angel::vec4(0.0, 0.0, 1.0, 1.0)); // done
	model2.setup();
	dummy2.addChild(box).addChild(selfRotAnim).addChild(model2);

	GroupNode dummy3;
	ModelNode model3(gallery.next(), Angel::vec4(1.0, 1.0, 0.0, 1.0)); // done
	model3.setup();
	dummy3.addChild(box).addChild(selfRotAnim).addChild(model3);
	
	GroupNode dummy4;
	ModelNode model4(gallery.next(), Angel::vec4(1.0, 0.0, 1.0, 1.0)); // done
	model4.setup();
	dummy4.addChild(box).addChild(selfRotAnim).addChild(model4);
	
	GroupNode dummy5;
	ModelNode model5(gallery.next(), Angel::vec4(0.0, 1.0, 1.0, 1.0)); // done
	model5.setup();
	dummy5.addChild(box).addChild(selfRotAnim).addChild(model5);
	
	GroupNode dummy6;
	ModelNode model6(gallery.next(), Angel::vec4(0.5, 0.55, 0.35, 1.0)); // done
	model6.setup();
	dummy6.addChild(box).addChild(selfRotAnim).addChild(model6);

	GroupNode dummy7;
	ModelNode model7(gallery.next(), Angel::vec4(0.3, 0.8, 0.5, 1.0)); // done
	model7.setup();
	dummy7.addChild(box).addChild(selfRotAnim).addChild(model7);

	ArmNode toRight(Angel::vec3(-0.38f, -0.35f, 0.0f), TransformNode::RIGHT);
	ArmNode toLeft(Angel::vec3(0.38f, -0.35f, 0.0f), TransformNode::RIGHT);

	SinusoidAnimator sinuAnim(500, TransformNode::Y_AXIS, 0.06f);
	animEngine.registerAnimator(sinuAnim);
	toRight.linkSinusoidAnimator(&sinuAnim);
	SinusoidAnimator sinuAnimOffset(500, TransformNode::Y_AXIS, 0.06f, 180.0f);
	animEngine.registerAnimator(sinuAnimOffset);
	toLeft.linkSinusoidAnimator(&sinuAnimOffset);
	// leftEnd.linkSinusoidAnimator(&sinuAnimOffset);

	GroupNode root;
	mainScene.setRoot(&root, Angel::vec3(0.0f, 0.72f, 0.0f));
	root.addChild(dummy);

	GroupNode grpLv1R; grpLv1R.addChild(rotAnimRever); root.addChild(grpLv1R);
	grpLv1R.addChild(toRight).addChild(dummy1);
	GroupNode grpLv1L; grpLv1L.addChild(rotAnimRever);
	root.addChild(grpLv1L);
	grpLv1L.addChild(toLeft).addChild(dummy2);

	GroupNode grpLv2R; grpLv2R.addChild(rotAnim); grpLv1L.addChild(grpLv2R);
	grpLv2R.addChild(toRight).addChild(dummy3);
	GroupNode grpLv2L; grpLv2L.addChild(rotAnim); grpLv1L.addChild(grpLv2L);
	grpLv2L.addChild(toLeft).addChild(dummy4);

	GroupNode grpLv3R; grpLv3R.addChild(rotAnimRever2); grpLv2L.addChild(grpLv3R);
	grpLv3R.addChild(toRight).addChild(dummy5);
	GroupNode grpLv3L; grpLv3L.addChild(rotAnimRever2); grpLv2L.addChild(grpLv3L);
	grpLv3L.addChild(toLeft).addChild(dummy6);

	GroupNode grpLv4; grpLv4.addChild(rotAnim);
	grpLv3R.addChild(grpLv4);
	grpLv4.addChild(toLeft).addChild(dummy7);


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
#include <cmath>
#include "AnimatorNode.h"

using namespace scn;
using namespace anim;

RotateAnimatorNode::RotateAnimatorNode(
	unsigned int periodFrame,
	enum TransformNode::Axis axis,
	enum RotateDirection direction,
	float startDegree,
	enum TransformNode::Side s ):
	TransformNode(Angel::identity(), s), axis(axis), 
	degree(startDegree), periodFrame(periodFrame)
{
	switch (direction)
	{
	case CLOCK:
		degPerFrame = -360.0f / (float)periodFrame;
	case CCLOCK:
	default:
		degPerFrame = 360.0f / (float)periodFrame;
		break;
	}
	updateMat();
}


RotateAnimatorNode::~RotateAnimatorNode()
{
}

void RotateAnimatorNode::play(std::chrono::microseconds ms, 
	unsigned int frmNum)
{
	degree = fmodf(degree + degPerFrame, 360.0f);
	updateMat();
}

void RotateAnimatorNode::action(Scene & scene)
{
	TransformNode::action(scene);
}

void RotateAnimatorNode::updateMat()
{
	switch (axis)
	{
	case TransformNode::X_AXIS:
		TransformNode::transformMat = Angel::RotateX(degree);
		break;
	case TransformNode::Y_AXIS:
		TransformNode::transformMat = Angel::RotateY(degree);
		break;
	case TransformNode::Z_AXIS:
		TransformNode::transformMat = Angel::RotateZ(degree);
		break;
	default:
		break;
	}
}

void RotateAnimatorNode::reverse()
{
	degPerFrame *= -1.0f;
}

float RotateAnimatorNode::getRate() const
{
	return this->degPerFrame;
}

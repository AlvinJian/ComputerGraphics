#include <cmath>
#include "AnimatorNode.h"

using namespace assignment3;

RotateAnimatorNode::RotateAnimatorNode(
	unsigned int periodFrame,
	enum RotateAxis axis,
	enum RotateDirection direction,
	float startDegree,
	enum TransformNode::Side s ):
	TransformNode(Angel::identity(), s), axis(axis), 
	degree(startDegree), periodFrame(periodFrame),
	direction(direction)
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

void RotateAnimatorNode::action(SceneGraph & scene)
{
	TransformNode::action(scene);
}

void RotateAnimatorNode::updateMat()
{
	switch (axis)
	{
	case RotateAnimatorNode::X_AXIS:
		TransformNode::transformMat = Angel::RotateX(degree);
		break;
	case RotateAnimatorNode::Y_AXIS:
		TransformNode::transformMat = Angel::RotateY(degree);
		break;
	case RotateAnimatorNode::Z_AXIS:
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

enum RotateAnimatorNode::RotateDirection 
	RotateAnimatorNode::getDirection() const
{
	return direction;
}

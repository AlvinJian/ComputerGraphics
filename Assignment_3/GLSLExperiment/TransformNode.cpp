#include "TransformNode.h"
#include "Scene.h"

using namespace assignment3;

TransformNode::TransformNode(Angel::mat4 matrix, Side side):
	transformMat(matrix), side(side)
{

}


TransformNode::~TransformNode()
{
}

void TransformNode::action(Scene & scene)
{
	if (side == TransformNode::LEFT)
	{
		scene.curModelMatrix = transformMat * scene.curModelMatrix;
	}
	else if (side == TransformNode::RIGHT)
	{
		scene.curModelMatrix = scene.curModelMatrix * transformMat;
	} else { }
}
#include "ModelNode.h"
#include "Scene.h"
#include "Angel.h"

using namespace assignment3;

ModelNode::ModelNode()
{
}


ModelNode::~ModelNode()
{
}

void ModelNode::action(Scene & scene)
{
	std::cout << __FUNCTION__ << std::endl;
	std::cout << scene.curModelMatrix * Angel::vec4(
		Angel::vec3()) << std::endl;
}
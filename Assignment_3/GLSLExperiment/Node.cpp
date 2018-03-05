#include "Node.h"
#include "Scene.h"
#include <iostream>

using namespace assignment3;

Node::Node()
{
}


Node::~Node()
{
}

GroupNode::GroupNode()
{

}

GroupNode::~GroupNode()
{

}

GroupNode& GroupNode::addChild(Node * c)
{
	childs.push_back(c);
	return *this;
}

const std::vector<Node*> & GroupNode::getChilds() const
{
	return childs;
}

void GroupNode::action(Scene & scene)
{
	std::cout << __FUNCTION__ << "===start===" << std::endl;
	scene.pushModelMatrix();
	scene.pushColorStack();
	for (auto i = 0; i < childs.size(); ++i)
	{
		childs[i]->action(scene);
	}
	scene.popModelMatrix();
	scene.popColorStack();
	std::cout << __FUNCTION__ << "===end===" << std::endl;
}
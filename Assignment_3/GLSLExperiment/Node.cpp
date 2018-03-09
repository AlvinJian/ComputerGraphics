#include "Node.h"
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

void GroupNode::action(SceneGraph & scene)
{
#if 0
	std::cout << __FUNCTION__ << "===start===" << std::endl;
#endif
	scene.pushModelMatrix();
	for (auto i = 0; i < childs.size(); ++i)
	{
		childs[i]->action(scene);
	}
	scene.popModelMatrix();
#if 0
	std::cout << __FUNCTION__ << "===end===" << std::endl;
#endif
}
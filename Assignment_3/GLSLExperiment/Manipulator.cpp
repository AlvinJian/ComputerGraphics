#include "Manipulator.h"
#include "Scene.h"
#include "ModelNode.h"

using namespace assignment3;
using namespace common;

void Manipulator::KbEventCallback(unsigned char key, int x, int y)
{
	if (key == 033)
	{
		exit(EXIT_SUCCESS);
	}
	if (InUse == nullptr)
	{
		return;
	}
	auto funcMap = InUse->getFuncMap();
	if (funcMap.find(key) != funcMap.end())
	{
		funcMap[key](key, x, y);
	}
}

const std::map<char, KbEventHandler>& ManipulatorImpl::getFuncMap() const
{
	return funcMap;
}

ManipulatorImpl::ManipulatorImpl()
{
	KbEventHandler shadingChange = [this](unsigned char k, int x, int y)
	{
		bool needRefresh = false;
		switch (ModelNode::ShadingMode)
		{
		case ModelNode::PER_PIXEL:
			ModelNode::ShadingMode = ModelNode::FLAT;
			needRefresh = true;
			break;
		case ModelNode::FLAT:
			ModelNode::ShadingMode = ModelNode::PER_PIXEL;
			needRefresh = true;
			break;
		default:
			break;
		}
		if (needRefresh)
		{
			glutPostRedisplay();
		}
	};
	funcMap['s'] = shadingChange;
}


ManipulatorImpl::~ManipulatorImpl()
{
}

ManipulatorImpl * SingleUsage<ManipulatorImpl>::InUse = nullptr;

Manipulator::Manipulator():
	ManipulatorImpl()
{
}

Manipulator::~Manipulator()
{}
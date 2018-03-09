#include "Manipulator.h"
#include "Scene.h"
#include "ModelNode.h"

using namespace assignment3;
using namespace common;

/* Manipulator * Manipulator::pInUseInstance = nullptr;

void Manipulator::Use(Manipulator * pInstance)
{
	pInUseInstance = pInstance;
} */

void Manipulator::KbEventCallback(unsigned char key, int x, int y)
{
	if (key == 033)
	{
		exit(EXIT_SUCCESS);
	}

	ManipulatorImpl * pInUseInstance = GetCurrent();
	if (pInUseInstance == nullptr)
	{
		return;
	}
	auto funcMap = pInUseInstance->getFuncMap();
	if (funcMap.find(key) !=
		funcMap.end())
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

ManipulatorImpl * SingleUsage<ManipulatorImpl>::current = nullptr;

Manipulator::Manipulator():
	ManipulatorImpl()
{
}

Manipulator::~Manipulator()
{}
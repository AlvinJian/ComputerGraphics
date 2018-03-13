#include <iostream>
#include "Manipulator.h"
#include "Scene.h"
#include "ModelNode.h"

using namespace assignment3;
using namespace common;

#define LIGHT_ANGEL_INCR 12.0f

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
		switch (k)
		{
		case 'M':
			ModelNode::ShadingMode = ModelNode::FLAT;
			needRefresh = true;
			break;
		case 'm':
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
	funcMap['m'] = shadingChange;
	funcMap['M'] = shadingChange;

	KbEventHandler spotLightCtrl = [this](unsigned char k, int x, int y)
	{
		bool needRefresh = false;
		switch (k)
		{
		// increase
		case 'p':
		{
			Scene::LightAngle += LIGHT_ANGEL_INCR;
			if (Scene::LightAngle >= 360.0f)
			{
				Scene::LightAngle = 360.0f;
			}
			needRefresh = true;
		}
		break;
		// decrease
		case 'P':
		{
			Scene::LightAngle -= LIGHT_ANGEL_INCR;
			if (Scene::LightAngle <= 0.0f)
			{
				Scene::LightAngle = 0.0f;
			}
			needRefresh = true;
		}
		break;
		default:
			break;
		}
		if (needRefresh)
		{
			std::cout << "Light Angle=" << Scene::LightAngle << std::endl;
			glutPostRedisplay();
		}
	};
	funcMap['p'] = spotLightCtrl;
	funcMap['P'] = spotLightCtrl;
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
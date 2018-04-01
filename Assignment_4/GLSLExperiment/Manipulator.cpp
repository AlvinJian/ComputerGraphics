#include <iostream>
#include "Manipulator.h"
#include "Scene.h"
#include "ModelNode.h"
#include "SinusoidAnimator.h"
#include "SkelBoxNode.h"

using namespace scn;
using namespace utils;

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
	KbEventHandler reflectionToggle = [this](unsigned char k, int x, int y)
	{
		static bool isReflect = false;
		if (isReflect)
		{
			ModelNode::ShadingMode = ModelNode::PER_PIXEL;
		}
		else
		{
			ModelNode::ShadingMode = ModelNode::REFLECTION;
		}
		isReflect = !isReflect;
		glutPostRedisplay();
	};
	funcMap['C'] = reflectionToggle;

	KbEventHandler refractionToggle = [this](unsigned char k, int x, int y)
	{
		static bool isRefract = false;
		if (isRefract)
		{
			ModelNode::ShadingMode = ModelNode::PER_PIXEL;
		}
		else
		{
			ModelNode::ShadingMode = ModelNode::REFRACTION;
		}
		isRefract = !isRefract;
		glutPostRedisplay();
	};
	funcMap['D'] = refractionToggle;

	KbEventHandler spotLightCtrl = [this](unsigned char k, int x, int y)
	{
		SceneGraph * pScn = Scene::GetCurrent();
		if (pScn == nullptr) return;
		bool needRefresh = false;
		switch (k)
		{
		// increase
		case 'p':
		{
			pScn->LightAngle += LIGHT_ANGEL_INCR;
			if (pScn->LightAngle >= 360.0f)
			{
				pScn->LightAngle = 360.0f;
			}
			needRefresh = true;
		}
		break;
		// decrease
		case 'P':
		{
			pScn->LightAngle -= LIGHT_ANGEL_INCR;
			if (pScn->LightAngle <= 0.0f)
			{
				pScn->LightAngle = 0.0f;
			}
			needRefresh = true;
		}
		break;
		default:
			break;
		}
		if (needRefresh)
		{
			std::cout << "Light Angle=" << pScn->LightAngle << std::endl;
			glutPostRedisplay();
		}
	};
	funcMap['p'] = spotLightCtrl;
	funcMap['P'] = spotLightCtrl;

	KbEventHandler sinuMovCtrl = [this](unsigned char k, int x, int y)
	{
		SinusoidAnimator::Switch = !SinusoidAnimator::Switch;
		glutPostRedisplay();
	};
	funcMap['s'] = sinuMovCtrl;

	KbEventHandler boxCtrl = [this](unsigned char k, int x, int y)
	{
		SkelBoxNode::Switch = !SkelBoxNode::Switch;
		glutPostRedisplay();
	};
	funcMap['e'] = boxCtrl;
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
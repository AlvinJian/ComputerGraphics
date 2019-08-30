#include <iostream>
#include "Manipulator.h"
#include "Scene.h"
#include "ModelNode.h"
#include "SinusoidAnimator.h"
#include "SkelBoxNode.h"

using namespace scn;
using namespace utils;

#define LIGHT_ANGEL_INCR 12.0f

Manipulator * SingleUsage<Manipulator>::InUse = nullptr;

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

const std::map<char, KbEventHandler>& Manipulator::getFuncMap() const
{
	return funcMap;
}

Manipulator::Manipulator()
{
	KbEventHandler reflectionToggle = [this](unsigned char k, int x, int y)
	{
		if (ModelNode::ShadingMode == ModelNode::REFLECTION)
		{
			ModelNode::ShadingMode = ModelNode::PER_PIXEL;
			glutPostRedisplay();
		}
		else if (ModelNode::ShadingMode == ModelNode::PER_PIXEL)
		{
			ModelNode::ShadingMode = ModelNode::REFLECTION;
			glutPostRedisplay();
		}
	};
	funcMap['C'] = reflectionToggle;

	KbEventHandler refractionToggle = [this](unsigned char k, int x, int y)
	{
		if (ModelNode::ShadingMode == ModelNode::REFRACTION)
		{
			ModelNode::ShadingMode = ModelNode::PER_PIXEL;
			glutPostRedisplay();
		}
		else if (ModelNode::ShadingMode == ModelNode::PER_PIXEL)
		{
			ModelNode::ShadingMode = ModelNode::REFRACTION;
			glutPostRedisplay();
		}
	};
	funcMap['D'] = refractionToggle;

	KbEventHandler spotLightCtrl = [this](unsigned char k, int x, int y)
	{
		Scene * pScn = Scene::GetCurrent();
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
	// funcMap['p'] = spotLightCtrl;
	// funcMap['P'] = spotLightCtrl;

	KbEventHandler envCtrl = [this](unsigned char k, int x, int y)
	{
		Skybox & skybox = Scene::GetCurrent()->background;
		if (skybox.currentMode == Skybox::TEXTURE)
		{
			skybox.currentMode = Skybox::PLAIN;
		}
		else if (skybox.currentMode == Skybox::PLAIN)
		{
			skybox.currentMode = Skybox::TEXTURE;
		}
		glutPostRedisplay();
	};
	funcMap['B'] = envCtrl;

	KbEventHandler shadowCtrl = [this](unsigned char k, int x, int y)
	{
		Scene * pScene = Scene::GetCurrent();
		pScene->shadowToggle = !pScene->shadowToggle;
		glutPostRedisplay();
	};
	funcMap['A'] = shadowCtrl;
}

Manipulator::~Manipulator()
{
}

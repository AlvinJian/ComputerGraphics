#include <iostream>
#include "Manipulator.h"

using namespace ctrl;
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
	auto funcMap = InUse->getKeyFuncs();
	if (funcMap.find(key) != funcMap.end())
	{
		funcMap[key](key, x, y);
	}
}

const std::map<char, KbEventHandler>& ManipulatorImpl::getKeyFuncs() const
{
	return funcMap;
}

void ManipulatorImpl::addKeyFunc(char key, const KbEventHandler & func)
{
	funcMap[key] = func;
}

ManipulatorImpl::ManipulatorImpl()
{
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
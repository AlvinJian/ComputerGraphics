#include "Config.h"
#include <iostream>

using namespace config;

ViewportConfig * ViewportConfig::Instance()
{
	static ViewportConfig* instance = new ViewportConfig;
	return instance;
}

const ViewportConfig * ViewportConfig::getInstance()
{
	return Instance();
}

void ViewportConfig::Init()
{
	Instance();
}

void ViewportConfig::SetSize(int w, int h)
{
	auto instance = Instance();
	instance->width = w;
	instance->height = h;
	std::cout << __FUNCTION__ << " w=" << w << std::endl;
	std::cout << __FUNCTION__ << " h=" << h<< std::endl;
}

int ViewportConfig::GetWidth()
{
	auto instance = Instance();
	return instance->width;
}

int ViewportConfig::GetHeight()
{
	auto instance = Instance();
	return instance->height;
}

void ViewportConfig::SetPos(int x, int y)
{
	auto instance = Instance();
	instance->x = x;
	instance->y = y;
}

int ViewportConfig::GetPosX()
{
	auto instance = Instance();
	return instance->x;
}

int ViewportConfig::GetPosY()
{
	auto instance = Instance();
	return instance->y;
}
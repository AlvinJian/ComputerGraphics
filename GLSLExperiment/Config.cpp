#include "Config.h"
#include <iostream>

using namespace assignment1;

static ViewportConfig* instance = nullptr;

void ViewportConfig::Init()
{
	if (instance == nullptr)
	{
		instance = new ViewportConfig();
	}
}

void ViewportConfig::SetSize(int w, int h)
{
	if (instance == nullptr) return;
	instance->width = w;
	instance->height = h;
	std::cout << __FUNCTION__ << " w=" << w << std::endl;
	std::cout << __FUNCTION__ << " h=" << h<< std::endl;
}

int ViewportConfig::GetWidth()
{
	if (instance == nullptr) return -1;
	return instance->width;
}

int ViewportConfig::GetHeight()
{
	if (instance == nullptr) return -1;
	return instance->height;
}
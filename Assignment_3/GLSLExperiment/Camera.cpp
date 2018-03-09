#include "Camera.h"

using namespace assignment3;

Camera::Camera() :
	position(Angel::vec3(0.0f, 0.0f, -1.5f)),
	up(Angel::vec3(0.0f, 1.0f, 0.0f)),
	at(Angel::vec3())
{
}


Camera::~Camera()
{
}

Angel::mat4 Camera::createViewMat() const
{
	return Angel::LookAt(position, at, up);
}

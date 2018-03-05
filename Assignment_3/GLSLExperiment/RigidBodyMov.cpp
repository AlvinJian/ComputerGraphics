#include "RigidBodyMov.h"
#include <cmath>

using namespace assignment3;

RigidBodyMov::RigidBodyMov():
	transState(0.0f, 0.0f, 0.0f),
	rotState(0.0f, 0.0f, 0.0f)
{
}


RigidBodyMov::~RigidBodyMov()
{
}

Angel::mat4 RigidBodyMov::getTranslateMatrix() const
{
	return Angel::Translate(transState);
}

Angel::vec3& RigidBodyMov::getTranslate()
{
	return transState;
}

void RigidBodyMov::translate(float dx, float dy, float dz)
{
	Angel::vec3 dv(dx, dy, dz);
	transState += dv;
}

Angel::mat4 RigidBodyMov::getRotateMatrix() const
{
	return Angel::RotateX(rotState.x) * Angel::RotateY(rotState.y) *
		Angel::RotateZ(rotState.z);
}

Angel::vec3& RigidBodyMov::getRotate()
{
	return rotState;
}

void RigidBodyMov::rotate(float dx, float dy, float dz)
{
	Angel::vec3 dtheta(dx, dy, dz);
	rotState += dtheta;
	rotState.x = std::fmodf(rotState.x, 360.0f);
	rotState.y = std::fmodf(rotState.y, 360.0f);
	rotState.z = std::fmodf(rotState.z, 360.0f);
}
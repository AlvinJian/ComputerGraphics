#include "RigidBodyMov.h"

using namespace assignment2;

RigidBodyMov::RigidBodyMov():
	transState(0.0f, 0.0f, -1.55f),
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

const Angel::vec3& RigidBodyMov::getTranslate() const
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

const Angel::vec3& RigidBodyMov::getRotate() const
{
	return rotState;
}

void RigidBodyMov::rotate(float dx, float dy, float dz)
{
	Angel::vec3 dtheta(dx, dy, dz);
	rotState += dtheta;
}
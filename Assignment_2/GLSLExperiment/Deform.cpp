#include "Deform.h"

using namespace assignment2;

Deform::Deform():
	shearVal{0.0f},
	twistVal{0.0f}
{
}


Deform::~Deform()
{
}

void Deform::addShear(const ShearVal & val)
{
	shearVal[val.first] += val.second;
}

Angel::mat4 Deform::getShearMatrix() const
{
	Angel::mat4 ret = Angel::identity();
	for (int i = 0; i < 3; ++i)
	{
		size_t ax = (i + 1) % 3;
		ret[i][ax] += shearVal[i];
	}
	return ret;
}
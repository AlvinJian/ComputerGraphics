#include <cmath>
#include "SinusoidAnimator.h"

using namespace assignment3;

SinusoidAnimator::SinusoidAnimator(
	unsigned int periodFrame,
	TransformNode::Axis axis,
	float amplitude,
	float phaseOffset): 
	degree(phaseOffset), axis(axis), 
	amplitude(amplitude), periodFrame(periodFrame),
	movement(0.0f)
{
	degPerFrame = 360.0f / (float)periodFrame;
}


SinusoidAnimator::~SinusoidAnimator()
{
}

void SinusoidAnimator::play(std::chrono::microseconds ms, 
	unsigned int frmNum)
{
	degree += degPerFrame;
	float rad = degree * Angel::DegreesToRadians;
	movement = amplitude * std::sin(rad);
}

std::pair<enum TransformNode::Axis, float> SinusoidAnimator::getMovement() const
{
	return std::make_pair(axis, movement);
}

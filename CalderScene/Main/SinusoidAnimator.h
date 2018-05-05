#pragma once
#include "TransformNode.h"
#include "Animation.h"

namespace scn
{
	class SinusoidAnimator: public anim::Animator
	{
	public:
		static bool Switch;

		SinusoidAnimator(
			unsigned int periodFrame,
			TransformNode::Axis axis,
			float amplitude,
			float phaseOffset = 0.0f
		);
		virtual ~SinusoidAnimator();

		void play(std::chrono::microseconds ms,
			unsigned int frmNum) override;

		std::pair<enum TransformNode::Axis, float>
			getMovement() const;

	private:
		float degree;
		float phase;
		float degPerFrame;
		enum TransformNode::Axis axis;
		unsigned int periodFrame;
		float amplitude;
		float movement;
	};
}

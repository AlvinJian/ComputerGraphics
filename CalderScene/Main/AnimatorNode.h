#pragma once
#include "Animation.h"
#include "TransformNode.h"

namespace scn
{
	class RotateAnimatorNode: public anim::Animator, public TransformNode
	{
	public:
		enum RotateDirection
		{
			CLOCK = -1, CCLOCK = 1
		};

		RotateAnimatorNode(
			unsigned int periodFrame,
			enum TransformNode::Axis axis,
			enum RotateDirection direction = RotateAnimatorNode::CCLOCK,
			float startDegree = 0,
			enum TransformNode::Side side = TransformNode::RIGHT
		);
		
		virtual ~RotateAnimatorNode();

		void play(std::chrono::microseconds ms, 
			unsigned int frmNum) override;
		void action(Scene & scene) override;

		void updateMat();
		void reverse();
		float getRate() const;

	private:
		float degree;
		float degPerFrame;
		enum TransformNode::Axis axis;
		unsigned int periodFrame;
	};
}

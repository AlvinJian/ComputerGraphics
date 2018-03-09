#pragma once
#include "Animation.h"
#include "TransformNode.h"

namespace assignment3
{
	class RotateAnimatorNode: public Animator, public TransformNode
	{
	public:
		enum RotateAxis 
		{
			X_AXIS = 0, Y_AXIS, Z_AXIS
		};
		enum RotateDirection
		{
			CLOCK = -1, CCLOCK = 1
		};

		RotateAnimatorNode(
			unsigned int periodFrame,
			enum RotateAxis axis, 
			enum RotateDirection direction = RotateAnimatorNode::CCLOCK,
			float startDegree = 0,
			enum TransformNode::Side side = TransformNode::RIGHT
		);
		
		virtual ~RotateAnimatorNode();

		void play(std::chrono::microseconds ms, 
			unsigned int frmNum) override;
		void action(SceneGraph & scene) override;

		void updateMat();
		void reverse();
		enum RotateDirection getDirection() const;

	private:
		float degree;
		float degPerFrame;
		enum RotateAxis axis;
		enum RotateDirection direction;
		unsigned int periodFrame;
	};
}

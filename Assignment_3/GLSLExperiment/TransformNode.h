#pragma once
#include "Node.h"
#include "Angel.h"

namespace assignment3
{
	class TransformNode: public Node
	{
	public:
		enum Side
		{
			LEFT = 0, RIGHT
		};

		TransformNode(Angel::mat4, Side side=RIGHT);
		virtual ~TransformNode();

		void action(Scene & scene) override;

	protected:
		Side side;
		Angel::mat4 transformMat;
	};
}


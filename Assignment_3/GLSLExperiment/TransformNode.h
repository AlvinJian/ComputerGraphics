#pragma once
#include "Node.h"
#include "Angel.h"

namespace scn
{
	class TransformNode: public Node
	{
	public:
		enum Side
		{
			LEFT = 0, RIGHT
		};

		enum Axis
		{
			X_AXIS = 0, Y_AXIS, Z_AXIS
		};

		TransformNode(Angel::mat4, Side side=RIGHT);
		TransformNode();
		virtual ~TransformNode();

		void action(SceneGraph & scene) override;

	protected:
		Side side;
		Angel::mat4 transformMat;
	};
}


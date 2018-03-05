#pragma once
#include "Node.h"
#include "PlyFile.h"

namespace assignment3
{
	class ModelNode: public Node
	{
	public:
		ModelNode(/* Ply& model */);
		virtual ~ModelNode();

		virtual void action(Scene & scene) override;
	};
}

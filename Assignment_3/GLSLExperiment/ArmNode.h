#pragma once
#include "TransformNode.h"
#include "Angel.h"

namespace assignment3
{
	class ArmNode: public TransformNode
	{
	public:
		ArmNode(const Angel::vec3 &, 
			TransformNode::Side s = TransformNode::RIGHT);
		virtual ~ArmNode();

		void action(Scene & scene) override;
	
	protected:
		size_t setupArms();
		Angel::vec3 directedShapeVec;

		Angel::vec4 color;

		GLuint vao;
		GLuint vbo;
		GLuint ebo;
		GLuint program;
	};
}

#pragma once
#include <utility>
#include "Angel.h"
#include "TransformNode.h"
#include "SinusoidAnimator.h"

namespace scn
{
	class ArmNode: public TransformNode
	{
	public:
		ArmNode(
			const Angel::vec3 &, 
			TransformNode::Side s = TransformNode::RIGHT);
		virtual ~ArmNode();

		void action(Scene & scene) override;

		// TODO ugly stuff
		void linkSinusoidAnimator(const SinusoidAnimator * animator);
	
	protected:
		size_t setupArms();
		Angel::vec3 directedShapeVec;

		Angel::vec4 color;

		GLuint vao;
		GLuint vbo;
		GLuint ebo;
		GLuint program;

		const SinusoidAnimator * sinuAnimator;
		Angel::mat4 armMat;
	};
}

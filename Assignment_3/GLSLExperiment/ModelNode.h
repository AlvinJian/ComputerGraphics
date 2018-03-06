#pragma once
#include "Node.h"
#include "PlyFile.h"

namespace assignment3
{
	class ModelNode: public Node
	{
	public:
		ModelNode(Ply & model, const Angel::vec4 & color);
		virtual ~ModelNode();

		virtual void action(Scene & scene) override;
		void setup();
		const Ply & getPlyModel() const
		{
			return plyModel;
		}

	private:
		// std::vector<Angel::vec4> genColor();

		Ply & plyModel;
		Angel::vec4 color;

		GLuint vao;
		GLuint vbo;
		GLuint ebo;
		GLuint program;
		size_t elementNum;
	};
}

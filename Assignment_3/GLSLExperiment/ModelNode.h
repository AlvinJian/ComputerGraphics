#pragma once
#include "Node.h"
#include "PlyFile.h"

namespace assignment3
{
	class ModelNode: public Node
	{
	public:
		enum SHADING_MODE
		{
			FLAT = 0, PER_PIXEL = 1
		};

		static int ShadingMode; // flat

		ModelNode(Ply & model, const Angel::vec4 & color);
		virtual ~ModelNode();

		virtual void action(SceneGraph & scene) override;
		void setup();
		const Ply & getPlyModel() const
		{
			return plyModel;
		}

	private:
		Ply & plyModel;
		Angel::vec4 color;

		GLuint vao;
		GLuint vbo;
		GLuint ebo;
		GLuint program;
		size_t elementNum;
	};
}

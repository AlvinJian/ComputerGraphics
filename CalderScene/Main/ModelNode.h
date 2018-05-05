#pragma once
#include "Node.h"
#include "PlyFile.h"

namespace scn
{
	class ModelNode: public Node
	{
	public:
		enum SHADING_MODE
		{
			FLAT = 0, PER_PIXEL = 1, REFLECTION=2, REFRACTION=3
		};

		static int ShadingMode; // flat

		ModelNode(common::Ply & model, const Angel::vec4 & color);
		virtual ~ModelNode();

		virtual void action(SceneGraph & scene) override;
		void setup();
		const common::Ply & getPlyModel() const
		{
			return plyModel;
		}

	private:
		common::Ply & plyModel;
		Angel::vec4 color;

		GLuint vao;
		GLuint vbo;
		GLuint ebo;
		GLuint program;
		size_t elementNum;

		GLuint shadowVao;
		GLuint shadowVbo;
		GLuint shadowEbo;
		GLuint shadowProgram;
	};
}

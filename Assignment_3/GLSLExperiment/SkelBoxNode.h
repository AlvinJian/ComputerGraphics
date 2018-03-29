#pragma once
#include <string>
#include "PlyFile.h"
#include "Node.h"
#include "Angel.h"

namespace scn
{
	class SkelBoxNode: public Node
	{
	public:
		static bool Switch;

		SkelBoxNode(const Angel::vec4 & color);
		virtual ~SkelBoxNode();

		void action(SceneGraph & scene) override;

	private:
		static std::string CubePlyPath;

		void setupBox();

		Angel::vec4 color;
		common::Ply * cubePly;


		GLuint vao;
		GLuint vbo;
		GLuint ebo;
		GLuint program;
	};
}

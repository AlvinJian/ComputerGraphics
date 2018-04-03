#pragma once
#include "Angel.h"
#include "PlyFile.h"
#include "bmpread.h"

namespace scn
{
	class SceneGraph;
	class Skybox
	{
	public:
		enum Mode
		{
			TEXTURE = 0, PLAIN
		};

		static std::string GrassBmp;
		static std::string StoneBmp;

		Skybox();
		~Skybox();

		Mode currentMode;
		
		void draw(SceneGraph & scene);
		void loadCubemap();
		void genPlainCube();
		void bindCubemap(bool);

	private:
		static std::string CubePlyPath;

		void setup();

		common::Ply * cubePly;

		GLuint vao;
		GLuint vbo;
		GLuint ebo;
		GLuint program;

		GLuint texture;
		GLuint plainTex;
	};
}

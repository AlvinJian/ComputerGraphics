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
		static int CurrentMode;

		enum Mode
		{
			TEXTURE = 0, PLAIN
		};

		static std::string GrassBmp;
		static std::string StoneBmp;

		Skybox();
		~Skybox();

		
		void draw(SceneGraph & scene);
		void loadCubemap();
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
	};
}

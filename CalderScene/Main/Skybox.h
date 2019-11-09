#pragma once
#include "Angel.h"
#include "PlyFile.h"
#include "bmpread.h"

namespace scn
{
	class Scene;
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

		void draw(Scene& scene);
		void loadCubemap();
		void genPlainCube();
		void loadEnvMap();
		void bindCubemap(bool);
		void bindEnvMap(bool);

	private:
		static std::string CubePlyPath;

		void setup();

		std::unique_ptr<common::Ply> cubePly;

		GLuint vao;
		GLuint vbo;
		GLuint ebo;
		GLuint program;

		GLuint texture;
		GLuint plainTex;
		GLuint envTex;
	};
}

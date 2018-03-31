#pragma once
#include "Angel.h"
#include "bmpread.h"

namespace scn
{
	class SceneGraph;
	class Skybox
	{
	public:
		static std::string GrassBmp;
		static std::string StoneBmp;

		Skybox();
		~Skybox();

		void setup();
		void draw(SceneGraph & scene);
		void loadTexture();

	private:
		GLuint vao, vbo;
		GLuint program;

		GLuint texture;
	};
}

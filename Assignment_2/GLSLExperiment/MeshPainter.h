#pragma once
#include <vector>
#include "Angel.h"
#include "PlyFile.h"

namespace assignment2 {
	using color4 = Angel::vec4;
	class MeshPainter
	{
	public:
		MeshPainter(std::vector<color4> & palette);
		~MeshPainter();

		void draw(Ply & plyModel);
		void setPalette(const std::vector<color4> & newPalette);

	private:
		static void calcMatrices();
		static void drawCallback();
		static void reshape(int w, int h);
		std::vector<color4> genColors(Ply & plyModel);

		std::vector<color4> palette;
		size_t elementNum;

		GLuint vao;
		GLuint vbo;
		GLuint ebo;
		GLuint program;
		Ply * model;
	};
}

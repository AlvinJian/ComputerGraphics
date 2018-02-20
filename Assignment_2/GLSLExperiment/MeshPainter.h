#pragma once
#include <vector>
#include "Angel.h"
#include "PlyFile.h"
#include "RigidBodyMov.h"
#include "Deform.h"

namespace assignment2 {
	using color4 = Angel::vec4;
	class MeshPainter
	{
	public:
		static MeshPainter * CurrentDrawingInstance();

		MeshPainter(const std::vector<color4> & palette);
		~MeshPainter();

		void draw(Ply & plyModel);
		void updateDraw();
		void setPalette(const std::vector<color4> & newPalette);

		// well, this is ugly
		RigidBodyMov rigid;
		Deform deform;

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

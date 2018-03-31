#pragma once
#include <string>
#include <memory>
#include "bmpread.h"
#include "Angel.h"

namespace scn
{
	class SceneGraph;
	class Plane
	{
	public:
		static std::string GrassBmp;
		static std::string StoneBmp;

		int createTexture(std::string&);
		void setup();
		void setModelMatrix(Angel::mat4 &);
		void draw(SceneGraph & scene);
		Plane();
		~Plane();

	private:
		int bmpRet;
		Angel::mat4 planeModelMat;

		GLuint texture;
		GLuint vao, ebo, vbo;
		size_t elemCount;
		GLuint program;
	};
}

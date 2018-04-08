#pragma once
#include <string>
#include "Angel.h"
#include "Utils.h"

#define PROGRAM_VERTEX_ATTRIBUTE 0
#define PROGRAM_TEXCOORD_ATTRIBUTE 1

namespace image
{
	class ImageRenderer
	{
	public:
		~ImageRenderer();

		void loadImageToTexture(const std::string &);
		void setPostProcessShader(GLuint postProcess);
		void render();
	
	protected:
		ImageRenderer();

		void setupPreProcess();
		void setupPostProcess();

		GLuint vao, vbo, ebo;
		GLuint texture;
		GLuint program;
		GLuint framebuffer;
		GLuint framebufferTexture;

		GLuint defaultPostProgram;
	};

	class SingleImageRender : public utils::SingleUsage<ImageRenderer>
	{
	public:
		static void Render();

		SingleImageRender();
		~SingleImageRender();
	};
}

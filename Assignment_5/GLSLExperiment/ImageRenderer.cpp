#include <iostream>
#include <vector>
#include "ImageRenderer.h"
#include "Config.h"
#include "bmpread.h"

using namespace image;

#define PROGRAM_VERTEX_ATTRIBUTE 0
#define PROGRAM_TEXCOORD_ATTRIBUTE 1

struct TextureData {
	Angel::vec4 position;
	Angel::vec2 texCoord;
};

ImageRenderer::ImageRenderer() :
	vao(0), vbo(0), ebo(0), program(0), texture(0),
	framebuffer(0), framebufferTexture(0),
	defaultPostProgram(0), currentPostProgram(0)
{
	defaultPostProgram = InitShader("vshader1.glsl", "fshader1.glsl");
	currentPostProgram = defaultPostProgram;
}


ImageRenderer::~ImageRenderer()
{
}

void ImageRenderer::loadImageToTexture(const std::string & filename)
{
	if (texture == 0)
	{
		glGenTextures(1, &texture);
	}
	glBindTexture(GL_TEXTURE_2D, texture);
#if 1
	int bmpRet;
	bmpread_t bitmap;
	bmpRet = bmpread(filename.c_str(), 0, &bitmap);
	if (!bmpRet)
	{
		std::cerr << "loading fails=" << filename << std::endl;
		system("pause");
		exit(1);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmap.width, bitmap.height,
		0, GL_RGB, GL_UNSIGNED_BYTE, bitmap.rgb_data);
	bmpread_free(&bitmap);
#endif
#if 0
	const std::vector<GLubyte> blue{ 84, 92, 128 };
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, blue.data());
#endif
	glBindTexture(GL_TEXTURE_2D, 0); // unbind
}

void image::ImageRenderer::setPostProcessShader(GLuint postProcess)
{
	currentPostProgram = postProcess;
}

void ImageRenderer::resetPostProcessShader()
{
	currentPostProgram = defaultPostProgram;
}

void ImageRenderer::render()
{
	glClearColor(0.33f, 0.33f, 0.1f, 1.0f);
	setupPreProcess();
	glClear(GL_COLOR_BUFFER_BIT);
	auto projLoc = glGetUniformLocation(program, "Projection");
	// Set our texture samples to the active texture unit
	mat4 projection = Ortho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	std::vector<GLfloat> projMatf = utils::FlattenMat4(projection);
	glUniformMatrix4fv(projLoc, 1, GL_TRUE, projMatf.data());
	glUniform1i(glGetUniformLocation(program, "texture"), 0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glFlush();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

#if 1
	// post process
	setupPostProcess();
	glClear(GL_COLOR_BUFFER_BIT);
	projLoc = glGetUniformLocation(currentPostProgram, "Projection");
	// Set our texture samples to the active texture unit
	glUniformMatrix4fv(projLoc, 1, GL_TRUE, projMatf.data());
	glUniform1i(glGetUniformLocation(currentPostProgram, "texture"), 0);
	glViewport(0, 0,
		config::ViewportConfig::GetWidth(),
		config::ViewportConfig::GetHeight()
	);
	glBindTexture(GL_TEXTURE_2D, framebufferTexture);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glFlush();
#endif

	glutSwapBuffers();
}

void ImageRenderer::setupPreProcess()
{
	// Create a vertex array object
	if (vao == 0)
	{
		glGenVertexArrays(1, &vao);
	}
	glBindVertexArray(vao);

	// create and bind vbo
	if (vbo == 0)
	{
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		float norm_h = 1.0f;

		// setup vertex buffer object
		std::vector<TextureData> coords;
		// bottom left;
		TextureData bl{
			Angel::vec4(-1.0f, -1.0f * norm_h, 0.0f, 1.0f), 
			Angel::vec2(0.0f, 0.0f)
		};
		coords.push_back((bl));

		// bottom right
		TextureData br{
			Angel::vec4(1.0f, -1.0f * norm_h, 0.0f, 1.0f), 
			Angel::vec2(1.0f, 0.0f)
		};
		coords.push_back((br));

		// top right
		TextureData tr{
			Angel::vec4(1.0f, 1.0f * norm_h, 0.0f, 1.0f), 
			Angel::vec2(1.0f, 1.0f)
		};
		coords.push_back(tr);

		// top left
		TextureData tl{
			Angel::vec4(-1.0f, 1.0f * norm_h, 0.0f, 1.0f), 
			Angel::vec2(0.0f, 1.0f)
		};
		coords.push_back(tl);
		glBufferData(GL_ARRAY_BUFFER, sizeof(TextureData) * coords.size(), 
			coords.data(), GL_STATIC_DRAW);
	}
	else
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
	}


	if (ebo == 0)
	{
		glGenBuffers(1, &ebo);
		// setup vertex element object
		// [bl, br, tr, tl]
		const std::vector<GLuint> indices{
			0, 1, 2,
			2, 3, 0
		};
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), 
			indices.data(), GL_STATIC_DRAW);
	}
	else
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	}
	
	if (program == 0)
	{
		program = InitShader("vshader1.glsl", "fshader1.glsl");
	}
	glUseProgram(program);

	glEnableVertexAttribArray(PROGRAM_VERTEX_ATTRIBUTE);
	glVertexAttribPointer(PROGRAM_VERTEX_ATTRIBUTE, 4, GL_FLOAT, GL_FALSE, 
		sizeof(TextureData), BUFFER_OFFSET(0));

	glEnableVertexAttribArray(PROGRAM_TEXCOORD_ATTRIBUTE);
	glVertexAttribPointer(PROGRAM_TEXCOORD_ATTRIBUTE, 2, GL_FLOAT, GL_FALSE, 
		sizeof(TextureData), BUFFER_OFFSET(sizeof(Angel::vec4)) );

#if 1
	// framebuffer
	if (framebuffer == 0)
	{
		glGenFramebuffers(1, &framebuffer);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// texture for framebuffer
	if (framebufferTexture == 0)
	{
		glGenTextures(1, &framebufferTexture);
	}
	glBindTexture(GL_TEXTURE_2D, framebufferTexture);
	GLsizei windowWidth = config::ViewportConfig::GetWidth();
	GLsizei windowHeight = config::ViewportConfig::GetHeight();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth, windowHeight,
		0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
		GL_TEXTURE_2D, framebufferTexture, 0);

	auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "Framebuffer is not complete!" << std::endl;
		system("pause");
		exit(1);
	}
#endif
}

void ImageRenderer::setupPostProcess()
{
	// re-use vao, vbo and ebo 
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glUseProgram(currentPostProgram);

	glEnableVertexAttribArray(PROGRAM_VERTEX_ATTRIBUTE);
	glVertexAttribPointer(PROGRAM_VERTEX_ATTRIBUTE, 4, GL_FLOAT, GL_FALSE,
		sizeof(TextureData), BUFFER_OFFSET(0));

	glEnableVertexAttribArray(PROGRAM_TEXCOORD_ATTRIBUTE);
	glVertexAttribPointer(PROGRAM_TEXCOORD_ATTRIBUTE, 2, GL_FLOAT, GL_FALSE,
		sizeof(TextureData), BUFFER_OFFSET(sizeof(Angel::vec4)));
}


ImageRenderer * utils::SingleUsage<ImageRenderer>::InUse = nullptr;

void SingleImageRender::Render()
{
	ImageRenderer * ptr = GetCurrent();
	if (ptr != nullptr)
	{
		ptr->render();
	}
}

SingleImageRender::SingleImageRender(): ImageRenderer()
{
}

SingleImageRender::~SingleImageRender()
{
}

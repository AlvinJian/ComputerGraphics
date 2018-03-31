#include <string>
#include <vector>
#include "Skybox.h"
#include "Scene.h"
#include "Config.h"

using namespace scn;

std::string Skybox::GrassBmp = "grass.bmp";
std::string Skybox::StoneBmp = "stones.bmp";

Skybox::Skybox():
	vao(0), vbo(0), program(0),
	texture(0)
{
}


Skybox::~Skybox()
{
}

void Skybox::setup()
{
	GLenum err = 0;
	int max_attribs;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_attribs);
	program = Angel::InitShader("skybox_vs.glsl", "skybox_fs.glsl");
	glUseProgram(program);
	auto vPosLoc = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosLoc);
	err = glGetError();
	std::cout << __FUNCTION__ << ":glEnableVertexAttribArray err=" << err << std::endl;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	err = glGetError();
	std::cout << __FUNCTION__ << ":glVertexAttribPointer err=" << err << std::endl;
	glUseProgram(0);
}

void Skybox::draw(SceneGraph & scene)
{
	if (program == 0)
	{
		setup();
	}
	GLenum err = 0;
	err = glGetError();
	std::cout << __FUNCTION__ << ": before glUseProgram err=" << err << std::endl;
	glUseProgram(program);
	err = glGetError();
	std::cout << __FUNCTION__ << ":glUseProgram err=" << err << std::endl;

	std::vector<GLfloat> skyboxVertices
	{
		// positions          
		-1.75f,  1.75f, -1.75f,
		-1.75f, -1.75f, -1.75f,
		1.75f, -1.75f, -1.75f,
		1.75f, -1.75f, -1.75f,
		1.75f,  1.75f, -1.75f,
		-1.75f,  1.75f, -1.75f,

		-1.75f, -1.75f,  1.75f,
		-1.75f, -1.75f, -1.75f,
		-1.75f,  1.75f, -1.75f,
		-1.75f,  1.75f, -1.75f,
		-1.75f,  1.75f,  1.75f,
		-1.75f, -1.75f,  1.75f,

		1.75f, -1.75f, -1.75f,
		1.75f, -1.75f,  1.75f,
		1.75f,  1.75f,  1.75f,
		1.75f,  1.75f,  1.75f,
		1.75f,  1.75f, -1.75f,
		1.75f, -1.75f, -1.75f,

		-1.75f, -1.75f,  1.75f,
		-1.75f,  1.75f,  1.75f,
		1.75f,  1.75f,  1.75f,
		1.75f,  1.75f,  1.75f,
		1.75f, -1.75f,  1.75f,
		-1.75f, -1.75f,  1.75f,

		-1.75f,  1.75f, -1.75f,
		1.75f,  1.75f, -1.75f,
		1.75f,  1.75f,  1.75f,
		1.75f,  1.75f,  1.75f,
		-1.75f,  1.75f,  1.75f,
		-1.75f,  1.75f, -1.75f,

		-1.75f, -1.75f, -1.75f,
		-1.75f, -1.75f,  1.75f,
		1.75f, -1.75f, -1.75f,
		1.75f, -1.75f, -1.75f,
		-1.75f, -1.75f,  1.75f,
		1.75f, -1.75f,  1.75f
	};

	if (vao == 0)
	{
		glGenVertexArrays(1, &vao);
	}
	glBindVertexArray(vao);
	if (vbo == 0)
	{
		glGenBuffers(1, &vbo);
	}
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	err = glGetError();
	std::cout << __FUNCTION__ << ":glBindBuffer err=" << err << std::endl;
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * skyboxVertices.size(),
		skyboxVertices.data(), GL_STATIC_DRAW);
	err = glGetError();
	std::cout << __FUNCTION__ << ":glBufferData err=" << err << std::endl;

	// projection matrix
	GLfloat ratio = (GLfloat)config::ViewportConfig::GetWidth() /
		(GLfloat)config::ViewportConfig::GetHeight();
	Angel::mat4 perspectiveMat = Angel::Perspective((GLfloat)45.0f, ratio, (
		GLfloat)0.1, (GLfloat) 100.0);
	auto perspectiveMatT = Angel::transpose(perspectiveMat);
	std::vector<float> projMatrixf = utils::FlattenMat4(perspectiveMatT);

	// view matrix
	Angel::mat4 viewMatrix = scene.camera.createViewMat();
	auto viewMatrixT = Angel::transpose(viewMatrix);
	std::vector<float> viewMatrixf = utils::FlattenMat4(viewMatrixT);

	glUniform1i(glGetUniformLocation(program, "skybox"), 1);

	GLuint viewMatrixLoc = glGetUniformLocation(program, "viewMatrix");
	glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, viewMatrixf.data());
	GLuint projMatrixLoc = glGetUniformLocation(program, "projectionMatrix");
	glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, projMatrixf.data());

	glDepthFunc(GL_LEQUAL);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	err = glGetError();
	std::cout << __FUNCTION__ << ":glDrawArrays err=" << err << std::endl;
	glFlush();
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0); // unbind
	glUseProgram(0);
	glDepthFunc(GL_LESS);
}

void Skybox::loadTexture()
{
	std::string faceBmpFiles[6] =
	{
		StoneBmp, // right
		StoneBmp, // left
		StoneBmp, // top
		GrassBmp, // bottom
		StoneBmp, // front
		StoneBmp, // back
	};
	glActiveTexture(GL_TEXTURE0);
	if (texture == 0)
	{
		glGenTextures(1, &texture);
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	int bmpRet;
	for (size_t i = 0; i < 6; ++i)
	{
		std::string & fileName = faceBmpFiles[i];
		bmpread_t bitmap;
		bmpRet = bmpread(fileName.c_str(), 0, &bitmap);
		if (!bmpRet)
		{
			std::cerr << "loading fails=" << fileName << std::endl;
			exit(1);
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
			bitmap.width, bitmap.height, 0, GL_RGB, GL_UNSIGNED_BYTE,
			bitmap.rgb_data);
		auto err = glGetError();
		std::cout << __FUNCTION__ << "glTexImage2D err=" << err << std::endl;
		bmpread_free(&bitmap);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0); // unbind
}

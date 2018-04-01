#include <string>
#include <vector>
#include "Skybox.h"
#include "Scene.h"
#include "Config.h"

using namespace scn;
using namespace common;

std::string Skybox::GrassBmp = "grass.bmp";
std::string Skybox::StoneBmp = "stones256.bmp";
std::string Skybox::CubePlyPath = "cube2.ply";

Skybox::Skybox():
	vao(0), vbo(0), program(0),
	texture(0), currentMode(Skybox::TEXTURE)
{
	cubePly = Ply::Load(CubePlyPath);
}


Skybox::~Skybox()
{
}

void Skybox::setup()
{
	GLenum err = 0;

	const std::vector<point4> & vertices = cubePly->getVertices();
	const std::vector<GLuint> & elements = cubePly->getFlattenIndexesOfFaces();

	// Create a vertex array object
	if (vao == 0)
	{
		glGenVertexArrays(1, &vao);
	}
	glBindVertexArray(vao);

	// create buffer
	size_t vertBufSize = vertices.size() * sizeof(point4);
	if (vbo == 0)
	{
		glGenBuffers(1, &vbo);
	}
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertBufSize, vertices.data(), GL_STATIC_DRAW);

	// Create ebo for faces
	size_t elemByteSize = sizeof(GLuint) * elements.size();
	if (ebo == 0)
	{
		glGenBuffers(1, &ebo);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elemByteSize, elements.data(),
		GL_STATIC_DRAW);

	if (program == 0)
	{
		program = Angel::InitShader("skybox_vs.glsl", "skybox_fs.glsl");
	}
	glUseProgram(program);

	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
}

void Skybox::draw(SceneGraph & scene)
{
	GLenum err = 0;
	setup();
	size_t elementNum = cubePly->getFlattenIndexesOfFaces().size();

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

	// model matrix
	Angel::mat4 modelMat(scene.curModelMatrix);
	auto modelMatrixT = Angel::transpose(modelMat);
	std::vector<float> modelMatrixf = utils::FlattenMat4(modelMatrixT);

	GLuint modelMatrixLoc = glGetUniformLocationARB(program, "modelMatrix");
	glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrixf.data());
	GLuint viewMatrixLoc = glGetUniformLocationARB(program, "viewMatrix");
	glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, viewMatrixf.data());
	GLuint projMatrixLoc = glGetUniformLocationARB(program, "projectionMatrix");
	glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, projMatrixf.data());

	glUniform1i(glGetUniformLocation(program, "skybox"), 0);
	glUniform1i(glGetUniformLocation(program, "skyboxMode"), currentMode);

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glDrawElements(GL_QUADS, elementNum, GL_UNSIGNED_INT, 0);
	glFlush();
	glDepthFunc(GL_LESS);
	
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0); // unbind
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);
}

void Skybox::loadCubemap()
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
		bmpread_free(&bitmap);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0); // unbind
}

void Skybox::bindCubemap(bool b)
{
	if (b && texture > 0)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	}
	else
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0); // unbind
	}
}

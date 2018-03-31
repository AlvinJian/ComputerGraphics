#include <iostream>
#include <vector>
#include "Scene.h"
#include "Plane.h"
#include "Config.h"

using namespace scn;

std::string Plane::GrassBmp = "grass.bmp";
std::string Plane::StoneBmp = "stones.bmp";

int Plane::createTexture(std::string & fileName)
{
	bmpread_t bitmap;
	bmpRet = bmpread(fileName.c_str(), 0, &bitmap);
	if (!bmpRet)
	{
		std::cerr << "loading fails=" << fileName << std::endl;
		return 0;
	}
	glActiveTexture(GL_TEXTURE0);
	if (texture == 0)
	{
		glGenTextures(1, &texture);
	}
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmap.width, bitmap.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmap.rgb_data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0); // unbind
	bmpread_free(&bitmap);
	return 1;
}

void Plane::setup()
{
	struct TexturePoint
	{
		Angel::vec4 position;
		Angel::vec3 normal;
		Angel::vec2 texCoord;
	};

	if (vao == 0)
	{
		glGenBuffers(1, &vao);
	}
	glBindVertexArray(vao);

	std::vector<TexturePoint> coords;
	// bottom left;
	TexturePoint bl
	{
		Angel::vec4(-1.0f, -1.0f, 0.0f, 1.0f),
		Angel::vec3(0.0f, 0.0f, 1.0f),
		Angel::vec2(0.0f, 0.0f)
	};
	coords.push_back(bl);

	// bottom right
	TexturePoint br
	{
		Angel::vec4(1.0f, -1.0f, 0.0f, 1.0f),
		Angel::vec3(0.0f, 0.0f, 1.0f),
		Angel::vec2(1.0f, 0.0f)
	};
	coords.push_back(br);

	// top right
	TexturePoint tr
	{
		Angel::vec4(1.0f, 1.0f, 0.0f, 1.0f),
		Angel::vec3(0.0f, 0.0f, 1.0f),
		Angel::vec2(1.0f, 1.0f)
	};
	coords.push_back(tr);

	// top left
	TexturePoint tl
	{
		Angel::vec4(-1.0f, 1.0f, 0.0f, 1.0f),
		Angel::vec3(0.0f, 0.0f, 1.0f),
		Angel::vec2(0.0f, 1.0f)
	};
	coords.push_back(tl);
	if (vbo == 0)
	{
		glGenBuffers(1, &vbo);
	}
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TexturePoint) * coords.size(), coords.data(), GL_STATIC_DRAW);

	if (ebo == 0)
	{
		glGenBuffers(1, &ebo);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	const std::vector<GLuint> indices
	{
		0, 1, 2,
		2, 3, 0
	};
	elemCount = indices.size();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size() , indices.data(),
		GL_STATIC_DRAW);

	if (program == 0)
	{
		program = Angel::InitShader("plane_vertex.glsl", "plane_frag.glsl");
	}
	glUseProgram(program);
	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, sizeof(TexturePoint),
		BUFFER_OFFSET(0));

	GLuint vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, sizeof(TexturePoint),
		BUFFER_OFFSET( sizeof(Angel::vec4) ) );

	GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
	glEnableVertexAttribArray(vTexCoord);
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(TexturePoint),
		BUFFER_OFFSET( sizeof(Angel::vec4) + sizeof(Angel::vec3) ) );

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);
}

void Plane::setModelMatrix(Angel::mat4 & matrix)
{
	planeModelMat = Angel::mat4(matrix);
}

void Plane::draw(SceneGraph & scene)
{
	Angel::mat4 modelMat(scene.curModelMatrix * planeModelMat);
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
	auto modelMatrixT = Angel::transpose(modelMat);
	std::vector<float> modelMatrixf = utils::FlattenMat4(modelMatrixT);

	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glUseProgram(program);

	GLuint modelMatrixLoc = glGetUniformLocation(program, "modelMatrix");
	glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrixf.data());
	GLuint viewMatrixLoc = glGetUniformLocation(program, "viewMatrix");
	glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, viewMatrixf.data());
	GLuint projMatrixLoc = glGetUniformLocation(program, "projectionMatrix");
	glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, projMatrixf.data());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glEnable(GL_DEPTH_TEST);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glDisable(GL_DEPTH_TEST);
	glFlush();

	// disable all buffer and shader program
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);
}

Plane::Plane():vao(0), ebo(0), vbo(0),
	program(0), planeModelMat(Angel::identity()),
	elemCount(0)
{
}


Plane::~Plane()
{
}

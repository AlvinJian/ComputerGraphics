#include <algorithm>
#include "ModelNode.h"
#include "Scene.h"
#include "Angel.h"
#include "Config.h"

using namespace assignment3;
using color4 = Angel::vec4;

ModelNode::ModelNode(Ply & model, const Angel::vec4 & color):
	plyModel(model), vao(0), 
	vbo(0), ebo(0), 
	program(0), elementNum(0),
	color(color)
{
}


ModelNode::~ModelNode()
{
}

void ModelNode::setup()
{
	const std::vector<Angel::vec4> colors(plyModel.getVertices().size(),
		color);
	const std::vector<point4>& points = plyModel.getVertices();
	const std::vector<GLuint>& elements = plyModel.getFlattenIndexesOfFaces();

	// Create a vertex array object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create vbo for points and colors
	size_t pointsByteSize = sizeof(point4) * points.size();
	size_t colorsByteSize = sizeof(color4) * colors.size();
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, pointsByteSize + colorsByteSize,
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, pointsByteSize,
		points.data());
	glBufferSubData(GL_ARRAY_BUFFER, pointsByteSize, colorsByteSize,
		colors.data());

	// Create ebo for faces
	size_t elemByteSize = sizeof(int) * elements.size();
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elemByteSize, elements.data(),
		GL_STATIC_DRAW);
	elementNum = elements.size();

	// Load shaders and use the resulting shader program
	program = Angel::InitShader("vshader1.glsl", "fshader1.glsl");
	glUseProgram(program);
	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(pointsByteSize));

	// disable all buffer and shader program
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);
}

void ModelNode::action(Scene & scene)
{
	Angel::mat4 modelMat(scene.curModelMatrix);
	// projection matrix
	GLfloat ratio = (GLfloat)config::ViewportConfig::GetWidth() /
		(GLfloat)config::ViewportConfig::GetHeight();
	Angel::mat4 perspectiveMat = Angel::Perspective((GLfloat)45.0f, ratio, (
		GLfloat)0.1, (GLfloat) 100.0);
	float projMatrixf[16] = { 0.0 };
	projMatrixf[0] = perspectiveMat[0][0]; projMatrixf[4] = perspectiveMat[0][1];
	projMatrixf[1] = perspectiveMat[1][0]; projMatrixf[5] = perspectiveMat[1][1];
	projMatrixf[2] = perspectiveMat[2][0]; projMatrixf[6] = perspectiveMat[2][1];
	projMatrixf[3] = perspectiveMat[3][0]; projMatrixf[7] = perspectiveMat[3][1];

	projMatrixf[8] = perspectiveMat[0][2]; projMatrixf[12] = perspectiveMat[0][3];
	projMatrixf[9] = perspectiveMat[1][2]; projMatrixf[13] = perspectiveMat[1][3];
	projMatrixf[10] = perspectiveMat[2][2]; projMatrixf[14] = perspectiveMat[2][3];
	projMatrixf[11] = perspectiveMat[3][2]; projMatrixf[15] = perspectiveMat[3][3];

	// view matrix
	point4 _camPos(Angel::vec3(0.0f, 0.0f, -1.5));
	point4 _at(Angel::vec3(0.0f, 0.0f, 0.0f));
	point4 _up(Angel::vec3(0.0f, 1.0f, 0.0f));
	Angel::mat4 viewMatrix = Angel::LookAt(_camPos, _at, _up);

	// model & view matrix
	Angel::mat4 mvMatrix = viewMatrix * modelMat;
	float modelMatrixf[16] = { 0.0 };
	modelMatrixf[0] = mvMatrix[0][0]; modelMatrixf[4] = mvMatrix[0][1];
	modelMatrixf[1] = mvMatrix[1][0]; modelMatrixf[5] = mvMatrix[1][1];
	modelMatrixf[2] = mvMatrix[2][0]; modelMatrixf[6] = mvMatrix[2][1];
	modelMatrixf[3] = mvMatrix[3][0]; modelMatrixf[7] = mvMatrix[3][1];

	modelMatrixf[8] = mvMatrix[0][2]; modelMatrixf[12] = mvMatrix[0][3];
	modelMatrixf[9] = mvMatrix[1][2]; modelMatrixf[13] = mvMatrix[1][3];
	modelMatrixf[10] = mvMatrix[2][2]; modelMatrixf[14] = mvMatrix[2][3];
	modelMatrixf[11] = mvMatrix[3][2]; modelMatrixf[15] = mvMatrix[3][3];

	// ortho matrix
	const Ply& m = getPlyModel();
	const point3& c = m.getCenter();
	std::vector<GLfloat> len{ m.getWidth(), m.getHeight(), m.getDepth() };
	GLfloat maxLen = *std::max_element(len.begin(), len.end());
	maxLen *= 5.0f;
	GLfloat left = c.x - 0.5f * maxLen;
	GLfloat right = c.x + 0.5f * maxLen;
	GLfloat bottom = c.y - 0.5f * maxLen;
	GLfloat top = c.y + 0.5f * maxLen;
	GLfloat znear = c.z - 0.5f * maxLen;
	GLfloat zfar = c.z + 0.5f * maxLen;
	Angel::mat4 orthoMat = Angel::Ortho(left, right, bottom, top, znear, zfar);
	float orthMatf[16] = { 0.0 };
	orthMatf[0] = orthoMat[0][0]; orthMatf[4] = orthoMat[0][1];
	orthMatf[1] = orthoMat[1][0]; orthMatf[5] = orthoMat[1][1];
	orthMatf[2] = orthoMat[2][0]; orthMatf[6] = orthoMat[2][1];
	orthMatf[3] = orthoMat[3][0]; orthMatf[7] = orthoMat[3][1];

	orthMatf[8] = orthoMat[0][2]; orthMatf[12] = orthoMat[0][3];
	orthMatf[9] = orthoMat[1][2]; orthMatf[13] = orthoMat[1][3];
	orthMatf[10] = orthoMat[2][2]; orthMatf[14] = orthoMat[2][3];
	orthMatf[11] = orthoMat[3][2]; orthMatf[15] = orthoMat[3][3];

	glBindVertexArray(vao);
	// glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glUseProgram(program);

	GLuint modelMatrixLoc = glGetUniformLocationARB(program, "model_matrix");
	glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrixf);
	GLuint projMatrixLoc = glGetUniformLocationARB(program, "projection_matrix");
	glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, projMatrixf);
	GLuint orthMatrixLoc = glGetUniformLocationARB(program, "orth_matrix");
	glUniformMatrix4fv(orthMatrixLoc, 1, GL_FALSE, orthMatf);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	glDrawElements(GL_TRIANGLES, elementNum, GL_UNSIGNED_INT, 0);
	glDisable(GL_DEPTH_TEST);

	glFlush();
}

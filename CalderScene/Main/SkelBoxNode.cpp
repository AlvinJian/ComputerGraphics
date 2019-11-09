#include "SkelBoxNode.h"
#include "Config.h"

using namespace scn;
using namespace common;

std::string SkelBoxNode::CubePlyPath = "cube2.ply";
bool SkelBoxNode::Switch = false;

SkelBoxNode::SkelBoxNode(const Angel::vec4& color) :
	color(color), vao(0),
	vbo(0), ebo(0),
	program(0), cubePly(Ply::Load(CubePlyPath))
{
}


SkelBoxNode::~SkelBoxNode()
{
}

void SkelBoxNode::action(Scene& scene)
{
	if (!Switch) return;
	setupBox();
	size_t elementNum = cubePly->getFlattenIndexesOfFaces().size();
	Angel::mat4 modelMat(scene.curModelMatrix);
	// projection matrix
	GLfloat ratio = (GLfloat)config::ViewportConfig::GetWidth() /
		(GLfloat)config::ViewportConfig::GetHeight();
	Angel::mat4 perspectiveMat = Angel::Perspective((GLfloat)45.0f, ratio, (
		GLfloat)0.1, (GLfloat)100.0);
	auto perspectiveMatT = Angel::transpose(perspectiveMat);
	std::vector<float> projMatrixf = utils::FlattenMat4(perspectiveMatT);

	// view matrix
	Angel::mat4 viewMatrix = scene.camera.createViewMat();
	auto viewMatrixT = Angel::transpose(viewMatrix);
	std::vector<float> viewMatrixf = utils::FlattenMat4(viewMatrixT);

	// model matrix
	auto modelMatrixT = Angel::transpose(modelMat);
	std::vector<float> modelMatrixf = utils::FlattenMat4(modelMatrixT);

	// ortho matrix
	const Ply& m = *cubePly;
	Angel::mat4 orthoMat = m.createOrthoMat();
	auto orthoMatT = Angel::transpose(orthoMat);
	std::vector<float> orthMatf = utils::FlattenMat4(orthoMatT);

	glBindVertexArray(vao);
	// glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glUseProgram(program);

	GLuint modelMatrixLoc = glGetUniformLocationARB(program, "modelMatrix");
	glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrixf.data());
	GLuint viewMatrixLoc = glGetUniformLocationARB(program, "viewMatrix");
	glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, viewMatrixf.data());
	GLuint projMatrixLoc = glGetUniformLocationARB(program, "projectionMatrix");
	glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, projMatrixf.data());
	GLuint orthMatrixLoc = glGetUniformLocationARB(program, "orthoMatrix");
	glUniformMatrix4fv(orthMatrixLoc, 1, GL_FALSE, orthMatf.data());

	// drawing
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);
	glDrawElements(GL_QUADS, elementNum, GL_UNSIGNED_INT, 0);
	glDisable(GL_DEPTH_TEST);
	glFlush();

	// disable all buffer and shader program
	glBindVertexArray(0);
	// glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);
}

void SkelBoxNode::setupBox()
{
	const std::vector<point4>& vertices = cubePly->getVertices();
	const std::vector<GLuint>& elements = cubePly->getFlattenIndexesOfFaces();
	std::vector<Angel::vec4> colors(vertices.size(), this->color);

	// Create a vertex array object
	if (vao == 0)
	{
		glGenVertexArrays(1, &vao);
	}
	glBindVertexArray(vao);

	// create buffer
	size_t vertBufSize = vertices.size() * sizeof(point4);
	size_t colorsBufSize = colors.size() * sizeof(point4);
	size_t bufSize = vertBufSize + colorsBufSize;
	if (vbo == 0)
	{
		glGenBuffers(1, &vbo);
	}
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, bufSize, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertBufSize, vertices.data());
	glBufferSubData(GL_ARRAY_BUFFER, vertBufSize, colorsBufSize, colors.data());

	// Create ebo for faces
	size_t elemByteSize = sizeof(int) * elements.size();
	if (ebo == 0)
	{
		glGenBuffers(1, &ebo);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elemByteSize, elements.data(),
		GL_STATIC_DRAW);

	// shader program
	if (program == 0)
	{
		program = Angel::InitShader("skel_vshader.glsl", "skel_fshader.glsl");
	}
	glUseProgram(program);

	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));
	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(vertBufSize));
}

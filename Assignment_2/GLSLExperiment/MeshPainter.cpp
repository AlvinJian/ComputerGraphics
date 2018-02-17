#include <iostream>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include "MeshPainter.h"
#include "Config.h"

using namespace assignment2;

static  MeshPainter* currentInstance = nullptr;

MeshPainter::MeshPainter(std::vector<color4> & palette) :
	palette(palette), model(nullptr)
{
}


MeshPainter::~MeshPainter()
{
}

std::vector<color4> MeshPainter::genColors(Ply & plyModel)
{
	std::vector<color4> colors(plyModel.getVertices().size());
	for (int i = 0; i < colors.size(); ++i)
	{
		if (palette.size() < 2)
		{
			colors[i] = palette[0];
		}
		else
		{
			int _i = std::rand() % palette.size();
			colors[i] = palette[_i];
		}
	}
	return colors;
}

void MeshPainter::setPalette(const std::vector<color4> & newPalette)
{
	palette = std::vector<color4>(newPalette.begin(), newPalette.end());
}

void MeshPainter::drawCallback()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	int x = config::ViewportConfig::GetPosX();
	int y = config::ViewportConfig::GetPosY();
	int w = config::ViewportConfig::GetWidth();
	int h = config::ViewportConfig::GetHeight();
	glViewport(x, y, w, h);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);
	glDrawElements(GL_TRIANGLES, currentInstance->elementNum, GL_UNSIGNED_INT, 0);
	glDisable(GL_DEPTH_TEST);

	glFlush();
	glutSwapBuffers();
}

// #define RATIO
#define SQUARE
// #define AS_IS

void MeshPainter::reshape(int w, int h)
{
#if defined(SQUARE)
	int side = std::max(w, h);
	config::ViewportConfig::SetSize(side, side);
	config::ViewportConfig::SetPos((w - side) / 2, (h - side) / 2);
#elif defined(AS_IS)
	config::ViewportConfig::SetSize(w, h);
#elif defined(RATIO)
	int width = w;
	int height = h;
	float winRatio = ((float)w) / ((float)h);
	float modelRatio = currentInstance->model->getWidth() /
		currentInstance->model->getHeight();
	if (winRatio > modelRatio)
	{
		// keep height and re-calc width
		width = (int)((float)h * modelRatio);// h * W/H
	}
	else
	{
		// keep width and re-calc height
		height = (int)((float)w / modelRatio); // w * H/W
	}
	config::ViewportConfig::SetSize(width, height);
	config::ViewportConfig::SetPos((w - width) / 2, (h - height) / 2);
#else
	config::ViewportConfig::SetSize(w, h);
#endif
	MeshPainter::calcMatrices();
};

void MeshPainter::draw(Ply & plyModel)
{
	currentInstance = this;
	model = &plyModel;
	const std::vector<color4> colors = genColors(plyModel);
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

	// sets the default color to clear screen
	glClearColor(0.5f, 0.5f, 0.5f, 0.5f); // background
	elementNum = elements.size();

	reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	glutReshapeFunc(reshape);
	glutDisplayFunc(drawCallback);
}

void MeshPainter::calcMatrices()
{
	MeshPainter& instance = *currentInstance;

	GLfloat ratio = (GLfloat)config::ViewportConfig::GetWidth() /
		(GLfloat)config::ViewportConfig::GetHeight();
	Angel::mat4 perspectiveMat = Angel::Perspective((GLfloat)45.0f, ratio, (
		GLfloat)0.1, (GLfloat) 100.0);
	float projMatrixf[16];
	projMatrixf[0] = perspectiveMat[0][0]; projMatrixf[4] = perspectiveMat[0][1];
	projMatrixf[1] = perspectiveMat[1][0]; projMatrixf[5] = perspectiveMat[1][1];
	projMatrixf[2] = perspectiveMat[2][0]; projMatrixf[6] = perspectiveMat[2][1];
	projMatrixf[3] = perspectiveMat[3][0]; projMatrixf[7] = perspectiveMat[3][1];

	projMatrixf[8] = perspectiveMat[0][2]; projMatrixf[12] = perspectiveMat[0][3];
	projMatrixf[9] = perspectiveMat[1][2]; projMatrixf[13] = perspectiveMat[1][3];
	projMatrixf[10] = perspectiveMat[2][2]; projMatrixf[14] = perspectiveMat[2][3];
	projMatrixf[11] = perspectiveMat[3][2]; projMatrixf[15] = perspectiveMat[3][3];

	// TODO rotate, translate, shear and twist in runtime.
	Angel::mat4 twist = Angel::identity();
	Angel::mat4 shear = Angel::identity();
	Angel::mat4 modelMat = Angel::identity();
	modelMat = modelMat * Angel::Translate(0.0f, 0.0f, -1.0f) * Angel::RotateY(0.0f) * 
		Angel::RotateX(0.0f) * Angel::RotateZ(0.0f) * twist * shear;
	
	float modelMatrixf[16];
	modelMatrixf[0] = modelMat[0][0]; modelMatrixf[4] = modelMat[0][1];
	modelMatrixf[1] = modelMat[1][0]; modelMatrixf[5] = modelMat[1][1];
	modelMatrixf[2] = modelMat[2][0]; modelMatrixf[6] = modelMat[2][1];
	modelMatrixf[3] = modelMat[3][0]; modelMatrixf[7] = modelMat[3][1];

	modelMatrixf[8] = modelMat[0][2]; modelMatrixf[12] = modelMat[0][3];
	modelMatrixf[9] = modelMat[1][2]; modelMatrixf[13] = modelMat[1][3];
	modelMatrixf[10] = modelMat[2][2]; modelMatrixf[14] = modelMat[2][3];
	modelMatrixf[11] = modelMat[3][2]; modelMatrixf[15] = modelMat[3][3];
	
	const Ply& m = *instance.model;
	const point3& c = m.getCenter();
	std::vector<GLfloat> len{ m.getWidth(), m.getHeight(), m.getDepth() };
	GLfloat maxLen = *std::max_element(len.begin(), len.end());
	maxLen *= 1.2f;
	GLfloat left = c.x - 0.5f * maxLen;
	GLfloat right = c.x + 0.5f * maxLen;
	GLfloat bottom = c.y - 0.5f * maxLen;
	GLfloat top = c.y + 0.5f * maxLen;
	GLfloat znear = c.z - 0.5f * maxLen;
	GLfloat zfar = c.z + 0.5f * maxLen;
	Angel::mat4 orthoMat = Angel::Ortho(left, right, bottom, top, znear, zfar);
	float orthMatf[16];
	orthMatf[0] = orthoMat[0][0]; orthMatf[4] = orthoMat[0][1];
	orthMatf[1] = orthoMat[1][0]; orthMatf[5] = orthoMat[1][1];
	orthMatf[2] = orthoMat[2][0]; orthMatf[6] = orthoMat[2][1];
	orthMatf[3] = orthoMat[3][0]; orthMatf[7] = orthoMat[3][1];

	orthMatf[8] = orthoMat[0][2]; orthMatf[12] = orthoMat[0][3];
	orthMatf[9] = orthoMat[1][2]; orthMatf[13] = orthoMat[1][3];
	orthMatf[10] = orthoMat[2][2]; orthMatf[14] = orthoMat[2][3];
	orthMatf[11] = orthoMat[3][2]; orthMatf[15] = orthoMat[3][3];

	// set up projection matricies
	GLuint modelMatrixLoc = glGetUniformLocationARB(instance.program, "model_matrix");
	glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrixf);
	GLuint projMatrixLoc = glGetUniformLocationARB(instance.program, "projection_matrix");
	glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, projMatrixf);
	GLuint orthMatrixLoc = glGetUniformLocationARB(instance.program, "orth_matrix");
	glUniformMatrix4fv(orthMatrixLoc, 1, GL_FALSE, orthMatf);
}
#include <algorithm>
#include "SierpinskiGasket.h"
#include "Config.h"
#include "Input.h"

using namespace assignment1;

#define INI_LEN 1.5

SierpinskiGasket::SierpinskiGasket()
{
}


SierpinskiGasket::~SierpinskiGasket()
{
}

void SierpinskiGasket::generateTriangles(float min)
{
	minLen = min;
	Triangle tri{ Angel::vec2(0.0, INI_LEN / std::sqrtf(3.0f) - 0.2f),
		Angel::vec2(-0.5f*INI_LEN, -0.5f*INI_LEN / std::sqrtf(3.0f) - 0.2f),
		Angel::vec2(0.5f*INI_LEN, -0.5f*INI_LEN / std::sqrtf(3.0f) - 0.2f) };
	divideTriangle(tri);
}

void SierpinskiGasket::divideTriangle(SierpinskiGasket::Triangle& tri)
{
	if (tri.getSideLen(0) > minLen)
	{
		Angel::vec2 v0 = tri.vertices[0];
		Angel::vec2 v1 = tri.vertices[1];
		Angel::vec2 v2 = tri.vertices[2];
		Angel::vec2 v3 = tri.calcMidPoint(0);
		Angel::vec2 v4 = tri.calcMidPoint(1);
		Angel::vec2 v5 = tri.calcMidPoint(2);

		Triangle t1{ v0, v3, v5	};
		divideTriangle(t1);
		Triangle t2{ v3, v1, v4 };
		divideTriangle(t2);
		Triangle t3{ v5, v4, v2 };
		divideTriangle(t3);
	}
	else
	{
		for (auto& v : tri.vertices)
		{
			allVertices.push_back(v);
		}
	}

}

const std::vector<Angel::vec2>& SierpinskiGasket::getVertices() const
{
	return allVertices;
}

float SierpinskiGasket::Triangle::getSideLen(int index) const
{
	int index1 = (index + 1) % 3;
	Angel::vec2 delta = vertices[index1] - vertices[index];
	return Angel::length(delta);
}

Angel::vec2 SierpinskiGasket::Triangle::calcMidPoint(int index) const
{
	int index1 = (index + 1) % 3;
	return (vertices[index1] + vertices[index]) / 2;
}

void SierpinskiGasket::Draw()
{
	SierpinskiGasket gasket;
	gasket.generateTriangles(0.08f);
	auto points = gasket.getVertices();

	// init GPU buffer
	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Angel::vec2) * points.size(),
		points.data(), GL_STATIC_DRAW);

	// setup shader
	// Load shaders and use the resulting shader program
	auto program = Angel::InitShader("vshader1.glsl", "fshader1.glsl");
	glUseProgram(program);

	// Initialize the vertex position attribute from the vertex shader
	GLuint loc = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	// setup transformation
	auto ortho = Angel::Ortho2D(-1.0f, 1.0f, -1.0f, 1.0f);
	static GLuint ProjLoc = 0;
	ProjLoc = glGetUniformLocation(program, "Proj");
	glUniformMatrix4fv(ProjLoc, 1, GL_TRUE, ortho);

	glClearColor(1.0, 1.0, 1.0, 1.0);        // sets white as color used to clear screen

	static int numOfPts = points.size();
	auto display = [](void) {
		// All drawing happens in display function
		glClear(GL_COLOR_BUFFER_BIT); // clear window
		int width = ViewportConfig::GetWidth();
		int height = ViewportConfig::GetHeight();
		glViewport(ViewportConfig::GetPosX(), ViewportConfig::GetPosY(), 
			width, height);
		glDrawArrays(GL_TRIANGLES, 0, numOfPts);    // draw the points
		auto err = glGetError();
		std::cout << __FUNCTION__ << "err=" << err << std::endl;
		glFlush();	// force output to graphics hardware
		return;
	};

	auto reshape = [](int w, int h) {
		int width = std::min(w, h);
		int height = std::min(w, h);
		glClear(GL_COLOR_BUFFER_BIT); // clear window
		ViewportConfig::SetSize(width, height);
		ViewportConfig::SetPos((w - width) / 2, (h - height) / 2);
	};
	reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

	glutReshapeFunc(reshape);
	glutDisplayFunc(display); // Register display callback function
}
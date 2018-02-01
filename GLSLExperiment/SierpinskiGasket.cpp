#include "SierpinskiGasket.h"

using namespace assignment1;

#define INI_LEN 0.9

SierpinskiGasket::SierpinskiGasket()
{
}


SierpinskiGasket::~SierpinskiGasket()
{
}

void SierpinskiGasket::generateTriangles(float min)
{
	minLen = min;
	Triangle tri{Angel::vec2(0.0, INI_LEN) ,
		Angel::vec2(-INI_LEN, -INI_LEN) , Angel::vec2(INI_LEN, -INI_LEN) };
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

const std::vector<Angel::vec2>& SierpinskiGasket::getVertexData() const
{
	/* if (allVertices.size() > 0)
	{
		return allVertices.data();
	}
	else
	{
		return nullptr;
	} */
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
	auto points = gasket.getVertexData();

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

	glClearColor(1.0, 1.0, 1.0, 1.0);        // sets white as color used to clear screen


	static int numOfPts = points.size();
	auto display = [](void) {
		// All drawing happens in display function
		glClear(GL_COLOR_BUFFER_BIT);                // clear window
		glDrawArrays(GL_TRIANGLES, 0, numOfPts);    // draw the points
		glFlush();										// force output to graphics hardware
		return;
	};

	auto keyboard = [](unsigned char key, int x, int y) {
		// keyboard handler
		switch (key) {
		case 033:			// 033 is Escape key octal value
			exit(1);		// quit program
			break;
		}
	};

	glutDisplayFunc(display); // Register display callback function
	glutKeyboardFunc(keyboard); // Register keyboard callback function

	// enter the drawing loop
	glutMainLoop();
}
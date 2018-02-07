#include <cmath>
#include <vector>
#include <iterator>
#include <algorithm>
#include "KochSnowflake.h"
#include "Config.h"
#include "Input.h"

using namespace assignment1;

KochSnowflake::KochSnowflake(Angel::vec2& pt1, 
	Angel::vec2& pt2, Angel::vec2& pt3):
	currentIter(1)
{
	vertices.push_back(pt1);
	vertices.push_back(pt2);
	vertices.push_back(pt3);
}

void KochSnowflake::iterate()
{
	auto curIt = vertices.begin();
	bool theEnd = false;
	while (!theEnd) {
		/*            pt2
		 *          /     \
		 * next--pt3        pt1--cur
		 */
		auto nextIt = std::next(curIt);
		if (nextIt == vertices.end())
		{
			nextIt = vertices.begin();
			theEnd = true;
		}
		Angel::vec2 dVec = *nextIt - *curIt;
		dVec /= 3;
		Angel::vec2 pt1 = *curIt + dVec;
		Angel::vec2 pt2 = pt1 + rotByMinus60Degree(dVec);
		Angel::vec2 pt3 = pt1 + dVec;
		auto it = vertices.insert(nextIt, pt3);
		it = vertices.insert(it, pt2);
		it = vertices.insert(it, pt1);
		curIt = nextIt;
	}
	++currentIter;
}

KochSnowflake::~KochSnowflake()
{
}

const std::list<Angel::vec2>& KochSnowflake::getVertices() const
{
	return vertices;
}

int KochSnowflake::iterLevel() const
{
	return currentIter;
}


Angel::vec2 KochSnowflake::rotByMinus60Degree(const Angel::vec2& inVec)
{
	const float cosMinus60deg = std::cos(M_PI * -60.0f/180.0f);
	const float sinMinus60deg = std::sin(M_PI * -60.0f / 180.0f);
	const Angel::mat2 rotMat(cosMinus60deg, -1.0f*sinMinus60deg,
		sinMinus60deg, cosMinus60deg);
	return rotMat * inVec;
}

#define INI_LEN 0.55f

void KochSnowflake::Draw(int iteration)
{
	if (iteration < 1)
	{
		return;
	}
	KochSnowflake snowFlake(Angel::vec2(0.0, INI_LEN),
		Angel::vec2(-INI_LEN, -INI_LEN), Angel::vec2(INI_LEN, -INI_LEN));
	while (snowFlake.iterLevel() < iteration)
	{
		snowFlake.iterate();
	}
	auto verticesList = snowFlake.getVertices();
	std::vector<Angel::vec2> points(verticesList.begin(), verticesList.end());

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


	static int width = std::min(WINDOW_WIDTH, WINDOW_HEIGHT);
	static int height = std::min(WINDOW_WIDTH, WINDOW_HEIGHT);

	static size_t numOfPts = 0;
	numOfPts = points.size();
	auto display = [](void) {
		std::cout << __FUNCTION__ << "numOfPts=" << numOfPts << std::endl;
		// All drawing happens in display function
		glClear(GL_COLOR_BUFFER_BIT);                // clear window
		glViewport(0, 0, width, height);
		glDrawArrays(GL_LINE_LOOP, 0, numOfPts);    // draw the points
		auto err = glGetError();
		std::cout << __FUNCTION__ << "err=" << err << std::endl;
		glFlush();	// force output to graphics hardware
		return;
	};

	auto reshape = [](int w, int h) {
		width = std::min(w, h);
		height = std::min(w, h);
		glClear(GL_COLOR_BUFFER_BIT); // clear window
		glViewport(0, 0, width, height);
	};

	glutReshapeFunc(reshape);
	glutDisplayFunc(display); // Register display callback function
	glutKeyboardFunc(Input::KbEventHandler); // Register keyboard callback function

								// enter the drawing loop
	// glutMainLoop();
}
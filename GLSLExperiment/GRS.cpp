#include <fstream>
#include <sstream>
#include <memory>
#include <numeric>
#include <algorithm>
#include "GRS.h"
#include "Config.h"
#include "Input.h"

using namespace assignment1;

GRSParser::GRSParser()
{
}

GRSParser::~GRSParser()
{
}

GRSData::GRSData()
{
}

GRSData::~GRSData()
{
	std::cout << __FUNCTION__ << std::endl;
}

GRSData* GRSParser::parse(const std::string& path)
{
	std::ifstream f(path);
	GRSData* data = new GRSData();
	std::string buffer;
	
	const unsigned char COMMENT = 0;
	const unsigned char MARGIN = 1;
	const unsigned char NUM_POLY = 2;
	const unsigned char POLY_PT = 3;

	unsigned char state = COMMENT;
	int ptNum = 0;
	while (std::getline(f, buffer))
	{
		switch (state)
		{
		case COMMENT:
		{
			if (buffer.find('*') != std::string::npos)
			{
				state = MARGIN;
			}
			buffer.clear();
		}
		break;
		
		case MARGIN:
		{
			std::istringstream stream(buffer);
			stream >> data->marginsLeft;
			stream >> data->marginsTop;
			stream >> data->marginRight;
			stream >> data->marginBottom;
			buffer.clear();
			state = NUM_POLY;
		}
		break;
		
		case NUM_POLY:
		{
			std::istringstream stream(buffer);
			stream >> data->numOfPolySegment;
			buffer.clear();
			state = POLY_PT;
		}
		break;

		case POLY_PT:
		{
			std::istringstream stream(buffer);
			if (ptNum == 0)
			{
				stream >> ptNum;
				data->polySegment.push_back(ptNum);
			}
			else
			{
				std::vector<Angel::vec2>& curPoly = data->polylines;
				Angel::vec2 pt;
				stream >> pt.x;
				stream >> pt.y;
				curPoly.push_back(std::move(pt));
				--ptNum;
			}
			buffer.clear();
		}
		break;
		
		default:
			break;
		}
	}

	//int t = std::accumulate(data->polySegment.begin(), data->polySegment.end(), 0);

	if (data->numOfPolySegment != data->polySegment.size())
	{
		std::cerr << "Err: data->numOfPolySegment != data->polySegment.size()" << std::endl;
	} 
	else
	{
		std::cout << "GRSParser::parse done!" << std::endl;
	}
	return data;
}

const Angel::vec2& GRSData::operator [] (int i) const
{
	return polylines[i];
}

size_t GRSData::size() const
{
	return polylines.size();
}

const std::vector<int>& GRSData::getPolySegmentSize() const
{
	return polySegment;
}

float GRSData::getWidth() const
{
	return marginRight - marginsLeft;
}

float GRSData::getHeight() const
{
	return marginsTop - marginBottom;
}

void GRSData::Draw(std::string& filePath)
{
	GRSParser parser;
	static std::unique_ptr<GRSData> dataPtr(nullptr);
	dataPtr = std::unique_ptr<GRSData>(parser.parse(filePath));
	GRSData& data = *(dataPtr.get());

	// init GPU buffer
	// Create a vertex array object
	static GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	static GLuint buffer = 0;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Angel::vec2) * data.size(),
		&(data[0]), GL_STATIC_DRAW);

	// setup shader
	// Load shaders and use the resulting shader program
	static GLuint program = 0;
	program = Angel::InitShader("vshader1.glsl", "fshader1.glsl");
	glUseProgram(program);

	// Initialize the vertex position attribute from the vertex shader
	static GLuint loc = 0;
	loc = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	
	// setup transformation
	auto ortho = Angel::Ortho2D(data.marginsLeft, data.marginRight,
		data.marginBottom, data.marginsTop);
	static GLuint ProjLoc = 0; 
	ProjLoc = glGetUniformLocation(program, "Proj");
	glUniformMatrix4fv(ProjLoc, 1, GL_TRUE, ortho);

	glClearColor(1.0, 1.0, 1.0, 1.0);        // sets white as color used to clear screen

	glBindVertexArray(0);

	static int width = -1;
	static int height = -1;
	auto reshape = [](int w, int h) {
		width = w; // std::min(w, h);
		height = h; // std::min(w, h);
		if (dataPtr.get() != nullptr) {
			float imgRatio = dataPtr->getWidth() / dataPtr->getHeight();
			float winRatio = ((float)w) / ((float)h);
			if (winRatio > imgRatio)
			{
				// remain height
				float r = dataPtr->getWidth() / dataPtr->getHeight();
				width = h * r;// h * W/H
			}
			else
			{
				// remain width
				float r = dataPtr->getHeight() / dataPtr->getWidth();
				height = w * r; // w * H/W
			}
		}
		glClear(GL_COLOR_BUFFER_BIT); // clear window
		glViewport(0, 0, width, height);
	};
	reshape(WINDOW_WIDTH, WINDOW_HEIGHT);

	auto display = [](void) {
		std::cout << "display" << std::endl;
		// All drawing happens in display function
		glClear(GL_COLOR_BUFFER_BIT); // clear window
		glBindVertexArray(vao);
		const std::vector<int>& polySegment = dataPtr->getPolySegmentSize();
		std::vector<GLint> offsets(polySegment.size());
		int offset = 0;
		for (auto i = 0; i < polySegment.size(); ++i)
		{
			offset = (i > 0)? offset+polySegment[i - 1]: 0;
			offsets[i] = offset;
		}
		int w = width / TILING_NUM;
		int h = height / TILING_NUM;
		for (int i = 0; i < TILING_NUM; ++i)
		{
			for (int j = 0; j < TILING_NUM; ++j)
			{
				glViewport(j*w, i*h, w, h);
				glMultiDrawArrays(GL_LINE_STRIP, 
					offsets.data(), polySegment.data(), polySegment.size());
			}
		}
		auto err = glGetError();
		std::cout << __FUNCTION__ << "err=" << err << std::endl;
		glFlush();
		glBindVertexArray(0);
		return;
	};

	glutReshapeFunc(reshape);

	glutDisplayFunc(display); // Register display callback function
	glutKeyboardFunc(Input::KbEventHandler); // Register keyboard callback function
}
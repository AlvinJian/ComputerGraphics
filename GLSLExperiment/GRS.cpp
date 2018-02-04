#include "GRS.h"
#include <fstream>
#include <sstream>
#include <memory>
#include "Config.h"

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
	size_t ptNum = 0;
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

const std::vector<size_t>& GRSData::getPolySegmentSize() const
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
	std::unique_ptr<GRSData> pdata(parser.parse(filePath));
	GRSData& data = *(pdata.get());

	// init GPU buffer
	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Angel::vec2) * data.size(),
		&(data[0]), GL_STATIC_DRAW);

	// setup shader
	// Load shaders and use the resulting shader program
	auto program = Angel::InitShader("vshader1.glsl", "fshader1.glsl");
	glUseProgram(program);

	// Initialize the vertex position attribute from the vertex shader
	GLuint loc = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glClearColor(1.0, 1.0, 1.0, 1.0);        // sets white as color used to clear screen

	static const GRSData* dataPtr = nullptr;
	dataPtr = pdata.get();
	auto display = [](void) {
		// All drawing happens in display function
		glClear(GL_COLOR_BUFFER_BIT); // clear window
		float hWratio = dataPtr->getHeight() / dataPtr->getWidth();
		int w = WINDOW_WIDTH;
		int h = static_cast<int>(WINDOW_WIDTH*hWratio);
		int x = (0 - w) / 2;
		int y = (0 - h) / 2;
		glViewport(x, y, w, h);
#if 0
		const std::vector<size_t>& polySegment = dataPtr->getPolySegmentSize();
		for (auto i = 0; i < polySegment.size(); ++i)
		{
			size_t offset = 0;
			if (i > 0)
			{
				offset = polySegment[i - 1];
			}
			glDrawArrays(GL_LINE_LOOP, offset, polySegment[i]);
			glFlush();
		}
#endif
#if 1
		glDrawArrays(GL_LINE_STRIP, 0, dataPtr->size());
		glFlush();
#endif
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
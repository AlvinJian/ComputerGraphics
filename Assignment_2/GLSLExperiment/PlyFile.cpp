#include <fstream>
#include <sstream>
#include <algorithm>
#include "PlyFile.h"

using namespace assignment2;

#define PLY_HEADER "ply"
#define HEADER_END "end_header"
#define ELEMENT_STR "element"
#define VERTEX_STR "vertex"
#define FACE_STR "face"

Ply::Ply():
	vertexNum(0), faceNum(0)
{
}

Ply::~Ply()
{
}

Ply* Ply::Load(const std::string & path)
{
	Ply * data = nullptr;
	std::ifstream f(path);
	std::string buffer;
	const char HEADER = 0;
	const char VERTEX_SEC = HEADER + 1;
	const char FACE_SEC = VERTEX_SEC + 1;
	const char END = -1;
	char state = HEADER;
	{
		// whether the first line is "ply"
		std::string magic(PLY_HEADER);
		std::getline(f, buffer);
		std::istringstream s(buffer);
		std::string line;
		s >> line;
		if (line != magic)
		{
			// if not, then return
			return data;
		}
		else
		{
			data = new Ply();
			buffer.clear();
		}
	}

	while (std::getline(f, buffer) && state != END)
	{
		switch (state)
		{
		case HEADER:
		{
			std::string tmp;
			std::istringstream stream(buffer);
			const std::string elem(ELEMENT_STR);
			const std::string end_header(HEADER_END);
			stream >> tmp;
			if (tmp == elem)
			{
				stream >> tmp;
				const std::string face(FACE_STR);
				const std::string vert(VERTEX_STR);
				if (tmp == face)
				{
					stream >> data->faceNum;
				}
				else if (tmp == vert)
				{
					stream >> data->vertexNum;
				}
				else
				{
				}
			}
			else if (tmp == end_header)
			{
				state = VERTEX_SEC;
			} else
			{
				// skip line
			}
		}
		buffer.clear();
		break;
		case VERTEX_SEC:
		{
			std::istringstream stream(buffer);
			point4 pt;
			stream >> pt.x;
			stream >> pt.y;
			stream >> pt.z;
			pt.w = 1.0f;
			data->vertices.push_back(pt);
			if (data->vertices.size() >= data->vertexNum)
			{
				state = FACE_SEC;
			}
		}
		buffer.clear();
		break;
		case FACE_SEC:
		{
			std::istringstream stream(buffer);
			int num = 0;
			stream >> num;
			std::vector<int> face(num);
			for (int i = 0; i < num; ++i)
			{
				stream >> face[i];
			}
			data->faces.push_back(face);
			if (data->faces.size() >= data->faceNum)
			{
				state = END;
			}
		}
		buffer.clear();
		break;
		case END:
		default:
			break;
		}
	}

	return data;
}
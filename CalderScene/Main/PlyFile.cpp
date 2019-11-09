#include <fstream>
#include <sstream>
#include <set>
#include <algorithm>
#include <functional>
#include "PlyFile.h"

using namespace common;

Ply::Ply():
	vertexNum(0), faceNum(0),
	width(0.0f), height(0.0f), 
	depth(0.0f), center(0.0f, 0.0f,0.0f)
{
}

void Ply::calcVertexNormal()
{
	normals = std::vector<Angel::vec3>(vertexNum, Angel::vec3());

	std::function<Angel::vec3(std::vector<Angel::vec3>&)> newellNormal = 
		[](std::vector<Angel::vec3>& vs) {
		Angel::vec3 normal;
		size_t num = vs.size();
		for (size_t i = 0; i < num; ++i)
		{
			size_t next_i = (i + 1) % num;
			normal.x += (vs[i].y - vs[next_i].y) * (vs[i].z + vs[next_i].z);
			normal.y += (vs[i].z - vs[next_i].z) * (vs[i].x + vs[next_i].x);
			normal.z += (vs[i].x - vs[next_i].x) * (vs[i].y + vs[next_i].y);
		}
		// return Angel::normalize(normal);
		return normal;
	};
#if 1
	std::vector<std::vector<size_t>> idFaces(vertexNum);
	fnorm = std::vector<Angel::vec3>(faceNum, Angel::vec3());
	for (size_t f=0; f<faces.size(); ++f)
	{
		std::vector<Angel::vec3> vs;
		for (auto id : faces[f])
		{
			idFaces[id].push_back(f);
			Angel::vec3 v(vertices[id].x, vertices[id].y, vertices[id].z);
			vs.push_back(v);
		}
		auto n = newellNormal(vs);
		auto nn = Angel::normalize(n);
		fnorm[f].x = nn.x;
		fnorm[f].y = nn.y;
		fnorm[f].z = nn.z;
	}
	
	for (size_t i = 0; i < idFaces.size(); ++i)
	{
		std::vector<size_t>& fidsPerVert = idFaces[i];
		Angel::vec3 avgNormal;
		for (auto f : fidsPerVert)
		{
			avgNormal += fnorm[f];
		}
		avgNormal /= (GLfloat)fidsPerVert.size();
		normals[i].x = avgNormal.x;
		normals[i].y = avgNormal.y;
		normals[i].z = avgNormal.z;
	}
	/* std::vector<Angel::vec3> dp(faceNum, Angel::vec3());
	for (size_t i=0; i<idFaces.size(); ++i)
	{
		std::vector<size_t>& fidsPerVert = idFaces[i];
		std::set<size_t> vids;
		for (auto f : fidsPerVert)
		{
			for (auto vid : faces[f])
			{
				vids.insert(vid);
			}
		}
		std::vector<Angel::vec3> vs;
		for (auto id: vids)
		{
			Angel::vec3 v(vertices[id].x, vertices[id].y, vertices[id].z);
			vs.push_back(v);
		}
		auto n = newellNormal(vs);
		normals[i].x = n.x;
		normals[i].y = n.y;
		normals[i].z = n.z;
	} */
#endif
}

Ply::~Ply()
{
}

std::unique_ptr<Ply> Ply::Load(const std::string & path)
{

  const std::string PLY_HEADER = "ply";
  const std::string HEADER_END = "end_header";
  const std::string ELEMENT_STR = "element";
  const std::string VERTEX_STR = "vertex";
  const std::string FACE_STR = "face";

  std::unique_ptr<Ply> data; 
	std::ifstream f(path);
	std::string buffer;
	const char HEADER = 0;
	const char VERTEX_SEC = HEADER + 1;
	const char FACE_SEC = VERTEX_SEC + 1;
	const char END = -1;
	char state = HEADER;
	{
		// whether the first line is "ply"
		const std::string magic(PLY_HEADER);
		std::getline(f, buffer);
		std::istringstream s(buffer);
		std::string line;
		s >> line;
		if (line != magic)
		{
			// if not, return
			return data;
		}
		else
		{
			data = std::unique_ptr<Ply>(new Ply());
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
			std::vector<GLuint> face(num);
			for (int i = 0; i < num; ++i)
			{
				stream >> face[i];
				data->flattenIndexesOfFaces.push_back(face[i]);
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
	// AdjustGeoCenterToOrigin(*data);
	data->calcGeoProperties();
	AdjustCenterToOrigin(*data);
	data->calcVertexNormal();
	data->name = std::string(path);
	return data;
}

const std::vector<point4>& Ply::getVertices() const
{
	return vertices;
}

const std::vector<Angel::vec3> Ply::getNormals() const
{
	return normals;
}

const std::vector<std::vector<GLuint>>& Ply::getFaces() const
{
	return faces;
}

void Ply::calcGeoProperties()
{
	auto cmpX = [](auto elem1, auto elem2)
	{
		return elem1.x < elem2.x;
	};
	auto xmax_it = std::max_element(vertices.begin(), vertices.end(), cmpX);
	auto xmin_it = std::min_element(vertices.begin(), vertices.end(), cmpX);
	GLfloat xmax = xmax_it->x;
	GLfloat xmin = xmin_it->x;
	width = xmax - xmin;

	auto cmpY = [](auto elem1, auto elem2)
	{
		return elem1.y < elem2.y;
	};
	auto ymax_it = std::max_element(vertices.begin(), vertices.end(), cmpY);
	auto ymin_it = std::min_element(vertices.begin(), vertices.end(), cmpY);
	GLfloat ymax = ymax_it->y;
	GLfloat ymin = ymin_it->y;
	height = ymax - ymin;

	auto cmpZ = [](auto elem1, auto elem2)
	{
		return elem1.z < elem2.z;
	};
	auto zmax_it = std::max_element(vertices.begin(), vertices.end(), cmpZ);
	auto zmin_it = std::min_element(vertices.begin(), vertices.end(), cmpZ);
	GLfloat zmax = zmax_it->z;
	GLfloat zmin = zmin_it->z;
	depth = zmax - zmin;

	center.x = 0.5f * (xmax + xmin);
	center.y = 0.5f * (ymax + ymin);
	center.z = 0.5f * (zmax + zmin);
}

const std::vector<GLuint>& Ply::getFlattenIndexesOfFaces() const
{
	return flattenIndexesOfFaces;
}

float Ply::getWidth() const
{
	return width;
}

float Ply::getHeight() const
{
	return height;
}

float Ply::getDepth() const
{
	return depth;
}

const point3& Ply::getCenter() const
{
	return center;
}

void Ply::AdjustGeoCenterToOrigin(Ply & ply)
{
	point3 gc;
	for (int i = 0; i < ply.vertexNum; ++i)
	{
		point3 p(ply.vertices[i].x, ply.vertices[i].y, ply.vertices[i].z);
		gc += p;
	}
	gc /= (float)ply.vertexNum;
	point4 gc4(gc, 0.0);
	for (int i = 0; i < ply.vertexNum; ++i)
	{
		ply.vertices[i] -= gc4;
	}
}

void Ply::AdjustCenterToOrigin(Ply & ply)
{
	point4 center4(ply.center, 0.0f);
	for (int i = 0; i < ply.vertexNum; ++i)
	{
		ply.vertices[i] -= center4;
	}
	ply.center = point3(0.0f, 0.0f, 0.0f);
}

const std::string& Ply::getName() const
{
	return name;
}

Angel::mat4 Ply::createOrthoMat(float factor) const
{
	const Ply& m = *this;
	const point3& c = m.getCenter();
	std::vector<GLfloat> len{ m.getWidth(), m.getHeight(), m.getDepth() };
	GLfloat maxLen = *std::max_element(len.begin(), len.end());
	maxLen *= factor;
	GLfloat left = c.x - 0.5f * maxLen;
	GLfloat right = c.x + 0.5f * maxLen;
	GLfloat bottom = c.y - 0.5f * maxLen;
	GLfloat top = c.y + 0.5f * maxLen;
	GLfloat znear = c.z + 0.5f * maxLen;
	GLfloat zfar = c.z - 0.5f * maxLen;
	return Angel::Ortho(left, right, bottom, top, znear, zfar);
}

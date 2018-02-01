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
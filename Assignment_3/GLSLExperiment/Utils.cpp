#include "Utils.h"

using namespace utils;

std::vector<float> utils::FlattenMat4(const Angel::mat4 & mat)
{
	size_t rowMax = 4;
	size_t colMax = 4;
	std::vector<float> ret(rowMax * colMax, 0.0f);
	for (size_t i = 0; i < rowMax; ++i)
	{
		for (size_t j = 0; j < colMax; ++j)
		{
			ret[rowMax*i + j] = mat[i][j];
		}
	}
	return ret;
}

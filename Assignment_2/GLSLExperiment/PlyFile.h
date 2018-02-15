#pragma once
#include <vector>
#include <string>
#include "Angel.h"

namespace assignment2 {
	using point4 = Angel::vec4;
	using color4 = Angel::vec4;
	
	class Ply;
	class PlyParser
	{
	public:
		explicit PlyParser();
		Ply * load(const std::string & path);
		~PlyParser();
	};

	class Ply {
	public:
		const std::vector<point4>& getVertices() const;
		const std::vector<std::vector<int>>& getPolygon() const;
		// TODO
		// std::vector<int> getFlattenPolygonInfo() const;
	};
}

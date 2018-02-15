#pragma once
#include <vector>
#include <string>
#include "Angel.h"

namespace assignment2 {
	using point4 = Angel::vec4;
	using point3 = Angel::vec3;
	using color4 = Angel::vec4;

	class Ply;
	class Ply {
	public:
		static Ply * Load(const std::string & path);
		~Ply();
		/* const std::vector<point4>& getVertices() const;
		const point4& vertexAt(int i) const;
		const std::vector<std::vector<int>>& getFaces() const;
		const std::vector<int>& faceAt(int i) const;
		std::vector<int> getFlattenFaces() const;
		float getWidth() const;
		float getHeight() const;
		point3 getFrontBottomLeftPoint() const; */

	private:
		explicit Ply();
		std::vector<point4> vertices;
		std::vector<std::vector<int>> faces;

		int vertexNum;
		int faceNum;
	};
}

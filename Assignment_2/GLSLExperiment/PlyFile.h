#pragma once
#include <vector>
#include <string>
#include "Angel.h"

namespace assignment2 {
	using point4 = Angel::vec4;
	using point3 = Angel::vec3;

	class Ply;
	class Ply {
	public:
		static Ply * Load(const std::string & path);
		~Ply();
		const std::vector<point4>& getVertices() const;
		const std::vector<std::vector<GLuint>>& getFaces() const;
		const std::vector<GLuint>& getFlattenIndexesOfFaces() const;
		float getWidth() const;
		float getHeight() const;
		float getDepth() const;
		const point3& getCenter() const;

	private:
		static void AdjustGeoCenterToOrigin(Ply &);
		explicit Ply();
		void calcGeoProperties();
		std::vector<point4> vertices;
		std::vector<std::vector<GLuint>> faces;
		std::vector<GLuint> flattenIndexesOfFaces;

		size_t vertexNum;
		size_t faceNum;
		GLfloat width;
		GLfloat height;
		GLfloat depth;
		point3 center;
	};
}

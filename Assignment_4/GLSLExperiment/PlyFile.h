#pragma once
#include <vector>
#include <string>
#include "Angel.h"

namespace common {
	using point4 = Angel::vec4;
	using point3 = Angel::vec3;

	class Ply;
	class Ply {
	public:
		static Ply * Load(const std::string & path);
		~Ply();
		const std::vector<point4>& getVertices() const;
		const std::vector<Angel::vec3> getNormals() const;
		const std::vector<std::vector<GLuint>>& getFaces() const;
		const std::vector<GLuint>& getFlattenIndexesOfFaces() const;
		float getWidth() const;
		float getHeight() const;
		float getDepth() const;
		const point3& getCenter() const;
		const std::string& getName() const;
		Angel::mat4 createOrthoMat(float factor=6.0f) const;

	private:
		static void AdjustGeoCenterToOrigin(Ply &);
		static void AdjustCenterToOrigin(Ply &);

		explicit Ply();
		void calcVertexNormal();
		void calcGeoProperties();
		std::vector<point4> vertices;
		std::vector<Angel::vec3> normals;
		std::vector<Angel::vec3> fnorm;
		std::vector<std::vector<GLuint>> faces;
		std::vector<GLuint> flattenIndexesOfFaces;

		size_t vertexNum;
		size_t faceNum;
		GLfloat width;
		GLfloat height;
		GLfloat depth;
		point3 center;

		std::string name;
	};
}

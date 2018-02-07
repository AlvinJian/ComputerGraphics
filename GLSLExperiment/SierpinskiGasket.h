#pragma once
#include "Angel.h"
#include <vector>

namespace assignment1 {
	class SierpinskiGasket
	{
	public:
		static void Draw();

		explicit SierpinskiGasket();
		~SierpinskiGasket();

		void generateTriangles(float min);
		const std::vector<Angel::vec2>& getVertices() const;

	private:
		float minLen;
		std::vector<Angel::vec2> allVertices;

		struct Triangle {
			// counterclockwise order
			Angel::vec2 vertices[3];
			float getSideLen(int index) const;
			Angel::vec2 calcMidPoint(int index) const;
		};

		void divideTriangle(Triangle& tri);
	};
}


#pragma once
#include "Angel.h"
#include <list>

namespace assignment1 {
	class KochSnowflake
	{
	public:
		static void Draw(int iteration);

		explicit KochSnowflake(Angel::vec2& pt1, Angel::vec2& pt2, Angel::vec2& pt3);
		~KochSnowflake();
		void iterate();
		const std::list<Angel::vec2>& getVertices() const;
		int iterLevel() const;

	private:
		std::list<Angel::vec2> vertices;
		int currentIter;

		static Angel::vec2 rotByMinus60Degree(const Angel::vec2&);
	};
}


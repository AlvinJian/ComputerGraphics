#pragma once
#include <string>
#include <vector>
#include "Angel.h"

namespace assignment1 {
	class GRSData;
	class GRSParser
	{
	public:
		explicit GRSParser();
		GRSData* parse(const std::string& path);
		~GRSParser();
	};

	class GRSData
	{
		friend class GRSParser;
	public:
		static void Draw(std::string& filePath);

		~GRSData();
		size_t size() const;
		const std::vector<size_t>& getPolySegmentSize() const;
		const Angel::vec2& operator [] (int i) const;
		float getWidth() const;
		float getHeight() const;

	private:
		explicit GRSData();
		float marginsLeft;
		float marginsTop;
		float marginRight;
		float marginBottom;
		size_t numOfPolySegment;
		std::vector<size_t> polySegment;
		std::vector<Angel::vec2> polylines;
	};
}
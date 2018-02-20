#pragma once
#include "PlyFile.h"
#include <string>
#include <utility>

namespace assignment2
{
	class Gallery
	{
	public:
		using PlyEntity = std::pair<std::string, Ply *>;
		Ply& current();
		Ply& next();
		Ply& prev();
		Gallery();
		~Gallery();

	private:
		static std::vector<std::string> LoadFilesWin32(const std::string &);

		void load();
		std::vector<PlyEntity> allPlys;
		const std::string dir;
		size_t _i;
	};
}


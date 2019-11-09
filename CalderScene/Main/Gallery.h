#pragma once
#include "PlyFile.h"
#include <string>
#include <utility>

namespace common
{
	class Gallery
	{
	public:
		using PlyEntity = std::pair<std::string, std::unique_ptr<Ply>>;
		Ply& current();
		Ply& next();
		Ply& prev();
		Gallery();
		~Gallery();

	private:
		static std::vector<std::string> LoadFilesWin32(const std::string&);

		void load();
		std::vector<PlyEntity> allPlys;
		const std::string dir;
		size_t _i;
	};
}


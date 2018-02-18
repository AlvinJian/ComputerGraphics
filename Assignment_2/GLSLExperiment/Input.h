#pragma once
#include <map>
#include <functional>
#include "MeshPainter.h"

namespace assignment2 {
	class Input
	{
		using KbFunc = std::function<void()>;
	public:
		static void InitKbFuncs();
		static void KbEventHandler(unsigned char key, int x, int y);

	private:
		static char prevKey;
		static char currentKey;
		static std::map<char, KbFunc> kbFuncsMapper;

		static void DoTranslation();
	};
}


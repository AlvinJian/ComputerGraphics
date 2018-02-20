#pragma once
#include <map>
#include <functional>
#include "MeshPainter.h"

namespace assignment2 {
	class Gallery;
	class Input
	{
		using KbFunc = std::function<void()>;
	public:
		static void InitKbFuncs();
		static void KbEventHandler(unsigned char key, int x, int y);
		static void SetGallery(Gallery *);

	private:
		static char prevKey;
		static char currentKey;
		static std::map<char, KbFunc> kbFuncsMapper;
		static Gallery * gallery;

		static void DoTranslation();
		static void DoShear();
		static void DoTwist();
		static void IteratePly();
	};
}


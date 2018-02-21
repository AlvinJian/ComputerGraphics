#pragma once
#include <map>
#include <functional>
#include "MeshPainter.h"

namespace assignment2 {
	class Gallery;
	class Manipulator
	{
		using KbFunc = std::function<void()>;
	public:
		static void InitKbFuncs();
		static void KbEventHandler(unsigned char key, int x, int y);
		static void SetGallery(Gallery *);

	private:
		static Gallery * gallery;

		static char prevKey;
		static char currentKey;
		static std::map<char, KbFunc> kbFuncsMapper;

		static int TranslateState;
		static void DoTranslation();

		static int ShearState;
		static void DoShear();

		static int TwistState;
		static void DoTwist();

		static void IteratePly();

		static int PaletteState;
		static void TogglePalette();

		static int SelfRotState;
		static void DoSelfRotate();
	};
}


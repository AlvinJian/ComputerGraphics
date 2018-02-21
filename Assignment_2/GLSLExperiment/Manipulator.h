#pragma once
#include <map>
#include <functional>
#include "MeshPainter.h"

namespace assignment2 {
	class Gallery;
	class Manipulator
	{
	public:
		using KbFunc = std::function<void()>;
		using IdleFunc = std::function<void()>;

		static void Init();
		static void KbEventHandler(unsigned char key, int x, int y);
		static void SetGallery(Gallery *);
		static void RegisterIdleFunctions(size_t reg, IdleFunc f);
		static void RunIdleFunctions();

	private:
		static Gallery * gallery;

		static char prevKey;
		static char currentKey;
		static std::map<char, KbFunc> kbFuncsMapper;

		static int TranslateState;
		static void DoTranslation();

		static void DoShear();

		static void DoTwist();

		static void IteratePly();

		static void TogglePalette();

		static int SelfRotState;
		static void DoSelfRotate();

		static void Reset();

		static std::vector<IdleFunc> funcs;
	};
}


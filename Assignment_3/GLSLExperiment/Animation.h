#pragma once
#include <map>
#include <chrono>
#include <ctime>
#include "Utils.h"

namespace anim
{
	using TimePoint = std::chrono::time_point<std::chrono::system_clock>;
	class Animator
	{
	public:
		Animator() {}
		virtual ~Animator() {}

		virtual void play(std::chrono::microseconds ms, 
			unsigned int frmNum) = 0;
	};

	class AnimationEngineImpl
	{
	public:
		~AnimationEngineImpl();

		int registerAnimator(Animator & anim);
		void deRegisterAnimator(int id);
		void playback();
		
	protected:
		AnimationEngineImpl(unsigned int fps = 30);
		int nextId;
		bool started;
		TimePoint startTime;
		unsigned int fps;
		unsigned int usPerFrame;
		unsigned int frameCount;
		TimePoint prevTime;
		std::map<int, Animator * > regAnimators;
	};

	class AnimationEngine : public utils::SingleUsage<AnimationEngineImpl>
	{
	public:
		static void Playback();

		AnimationEngine(unsigned int fps = 30);
		~AnimationEngine();
	};
}

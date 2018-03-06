#pragma once
#include <map>
#include <chrono>
#include <ctime>

namespace assignment3
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

	class AnimationEngine
	{
	public:
		static void Playback();
		static void Use(AnimationEngine *);

		AnimationEngine(unsigned int fps = 30);
		~AnimationEngine();

		int registerAnimator(Animator * anim);
		void deRegisterAnimator(int id);
		
	private:
		static AnimationEngine * InUse;

		int nextId;
		bool started;
		TimePoint startTime;
		unsigned int fps;
		unsigned int usPerFrame;
		unsigned int frameCount;
		TimePoint prevTime;
		std::map<int, Animator * > regAnimators;
	};
}

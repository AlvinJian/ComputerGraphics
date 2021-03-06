#include "Animation.h"
#include "Angel.h"

using namespace anim;
using namespace utils;

AnimationEngine::AnimationEngine(unsigned int fps):
	fps(fps), nextId(0),
	started(false), frameCount(0)
{
	usPerFrame = 1000000 / fps;
}

AnimationEngine::~AnimationEngine()
{
}

int AnimationEngine::registerAnimator(Animator & anim)
{
	regAnimators[nextId++] = &anim;
	return nextId - 1;
}

void AnimationEngine::deRegisterAnimator(int id)
{
	regAnimators.erase(id);
}

void AnimationEngine::playback()
{
	if (InUse != this) return;
	auto current = std::chrono::system_clock::now();
	bool shouldPlay = false;
	if (started)
	{
		// calculate time
		auto d = current - prevTime;
		std::chrono::microseconds duration_ms =
			std::chrono::duration_cast<std::chrono::microseconds>(d);
		if (duration_ms.count() >= usPerFrame)
		{
			shouldPlay = true;
		}
		else {
			shouldPlay = false;
		}
	}
	else
	{
		shouldPlay = true;
		started = true;
		startTime = current;
		frameCount = 0;
	}

	if (shouldPlay)
	{
		for (auto kv : regAnimators)
		{
			auto e = current - startTime;
			std::chrono::microseconds elaspe_ms =
				std::chrono::duration_cast<std::chrono::microseconds>(e);
			kv.second->play(elaspe_ms, frameCount);
		}
		prevTime = current;
		frameCount = (frameCount + 1 < 1000 * fps) ? frameCount + 1 : 0;
		glutPostRedisplay();
	}
}

AnimationEngine* SingleUsage<AnimationEngine>::InUse = nullptr;

void AnimationEngine::Playback()
{
	if (InUse != nullptr)
	{
		InUse->playback();
	}
}

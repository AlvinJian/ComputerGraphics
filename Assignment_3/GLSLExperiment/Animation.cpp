#include "Animation.h"
#include "Angel.h"

using namespace assignment3;

AnimationEngine * AnimationEngine::InUse = nullptr;

void AnimationEngine::Use(AnimationEngine * engine)
{
	if (InUse != nullptr) InUse->started = false;
	InUse = engine;
}

void AnimationEngine::Playback()
{
	if (InUse == nullptr) return;
	auto current = std::chrono::system_clock::now();
	bool shouldPlay = false;
	if (InUse->started)
	{
		// calculate time
		auto d = current - InUse->prevTime;
		std::chrono::microseconds duration_ms =
			std::chrono::duration_cast<std::chrono::microseconds>(d);
		if (duration_ms.count() >= InUse->usPerFrame)
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
		InUse->started = true;
		InUse->startTime = current;
		InUse->frameCount = 0;
	}

	if (shouldPlay)
	{
		for (auto kv : InUse->regAnimators)
		{
			auto e = current - InUse->startTime;
			std::chrono::microseconds elaspe_ms = 
				std::chrono::duration_cast<std::chrono::microseconds>(e);
			kv.second->play(elaspe_ms, InUse->frameCount);
		}
		InUse->prevTime = current;
		InUse->frameCount = (InUse->frameCount + 1 < 1000 * InUse->fps) ?
			InUse->frameCount + 1 : 0;
		glutPostRedisplay();
	}
}

AnimationEngine::AnimationEngine(unsigned int fps):
	fps(fps), nextId(0),
	started(false), frameCount(0)
{
	usPerFrame = 1000000 / fps;
}

AnimationEngine::~AnimationEngine()
{
}

int AnimationEngine::registerAnimator(Animator * anim)
{
	regAnimators[nextId++] = anim;
	return nextId - 1;
}

void AnimationEngine::deRegisterAnimator(int id)
{
	regAnimators.erase(id);
}
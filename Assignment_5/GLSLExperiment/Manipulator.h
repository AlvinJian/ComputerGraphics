#pragma once
#include <map>
#include <functional>
#include "Utils.h"

namespace ctrl
{
	using KbEventHandler =
		std::function<void(unsigned char, int, int)>;

	class ManipulatorImpl
	{
	public:
		const std::map<char, KbEventHandler> & getKeyFuncs() const;
		void addKeyFunc(char, const KbEventHandler &);
		~ManipulatorImpl();

	protected:
		ManipulatorImpl();
		std::map<char, KbEventHandler> funcMap;
	};

	class Manipulator : public utils::SingleUsage<ManipulatorImpl>
	{
	public:
		Manipulator();
		~Manipulator();
		static void KbEventCallback(unsigned char key, int x, int y);
	};
}

#pragma once
#include <map>
#include <functional>
#include "SingleUsage.h"

namespace assignment3
{
	using KbEventHandler =
		std::function<void(unsigned char, int, int)>;

	class ManipulatorImpl
	{
	public:
		const std::map<char, KbEventHandler> & getFuncMap() const;

		ManipulatorImpl();
		~ManipulatorImpl();

	private:
		std::map<char, KbEventHandler> funcMap;
	};

	class Manipulator : public common::SingleUsage<ManipulatorImpl>
	{
	public:
		Manipulator();
		~Manipulator();
		static void KbEventCallback(unsigned char key, int x, int y);
	};
}

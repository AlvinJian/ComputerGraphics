#pragma once
#include <map>
#include <functional>
#include "Utils.h"

namespace scn
{
	using KbEventHandler =
		std::function<void(unsigned char, int, int)>;

	class Manipulator : public utils::SingleUsage<Manipulator>
	{
	public:
		static void KbEventCallback(unsigned char key, int x, int y);

		Manipulator();
		const std::map<char, KbEventHandler> & getFuncMap() const;
		~Manipulator();

	private:
		std::map<char, KbEventHandler> funcMap;
	};
}

#pragma once
#include <vector>
#include "Angel.h"

namespace utils
{
	/*
	 * class Foobar: public SingleUsage<Foobar>
	 */
	template <class T>
	class SingleUsage
	{
	public:
		static inline T * GetCurrent()
		{
			return  InUse;
		}

		inline void use()
		{
			// NOTE: reinterpret_cast will cause problem in some class
			InUse = static_cast<T*>(this);
		}

		SingleUsage() = default;

		~SingleUsage() = default;
	
	protected:
		static T* InUse;
	};

	std::vector<float> FlattenMat4(const Angel::mat4 & mat);
}

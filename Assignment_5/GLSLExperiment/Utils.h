#pragma once
#include <vector>
#include "Angel.h"

namespace utils
{
	template <class T>
	class SingleUsage: public virtual T
	{
	public:
		static inline T * GetCurrent()
		{
			return  InUse;
		}

		inline void use()
		{
			InUse = this;
		}

		SingleUsage(){}

		~SingleUsage(){}
	
	protected:
		static T* InUse;
	};

	std::vector<float> FlattenMat4(const Angel::mat4 & mat);
}

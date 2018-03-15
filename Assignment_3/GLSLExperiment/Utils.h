#pragma once
#include <vector>

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


	template<typename outT, typename inT>
	inline std::vector<outT>
		Mat2DtoStdVec(const inT & mat2d, size_t rowMax, size_t colMax)
	{
		size_t num = rowMax * colMax;
		std::vector<outT> ret(num);
		for (auto i = 0; i < rowMax; ++i)
		{
			for (auto j = 0; j < colMax; ++j)
			{
				ret[rowMax*i + j] = mat2d[i][j];
			}
		}
		return ret;
	}
}

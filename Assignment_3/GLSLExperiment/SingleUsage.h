#pragma once
namespace common
{
	template <class T>
	class SingleUsage: public virtual T
	{
	public:
		static inline void Use(T * ptr)
		{
			InUse = ptr;
		}
		static inline T * GetCurrent()
		{
			return  InUse;
		}

		SingleUsage(){}

		~SingleUsage(){}
	
	protected:
		static T* InUse;
	};
}

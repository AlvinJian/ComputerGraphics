#pragma once
namespace common
{
	template <class T>
	class SingleUsage: virtual public T
	{
	public:
		static inline void Use(T * ptr)
		{
			current = ptr;
		}
		static inline T * GetCurrent()
		{
			return  current;
		}

		SingleUsage(){}

		~SingleUsage(){}
	
	protected:
		static T* current;
	};
}

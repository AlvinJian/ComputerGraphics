#pragma once
namespace common
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
}

#pragma once
#include <cstdlib>
#include <cassert>
#include <stdexcept>
#include <string>

namespace common
{
	template< class T>
	class Stack
	{
	public:
		Stack();
		~Stack();

		size_t size() const;
		bool empty() const;
		void push(const T & item);
		void pop();
		const T & top() const;

	private:
		static const size_t SIZE_INCR = 16;

		size_t count;
		size_t capacity;

		void enlarge();

		T * pData;
	};
	template<class T>
	inline Stack<T>::Stack(): count(0)
	{
		pData = reinterpret_cast<T*>(malloc(sizeof(T) * SIZE_INCR));
		memset(pData, 0, sizeof(T) * SIZE_INCR);
		capacity = SIZE_INCR;
	}
	template<class T>
	inline Stack<T>::~Stack()
	{
		free(pData);
	}
	template<class T>
	inline size_t Stack<T>::size() const
	{
		return count;
	}
	template<class T>
	inline bool Stack<T>::empty() const
	{
		return count < 1;
	}
	template<class T>
	inline void Stack<T>::push(const T & item)
	{
		memcpy(&pData[count++], &item, sizeof(T));
		if (count >= capacity)
		{
			enlarge();
		}
	}
	template<class T>
	inline void Stack<T>::pop()
	{
		if (count > 0)
		{
			--count; 
		}
		else
		{
			throw std::length_error("the stack is empty");
		}
	}
	template<class T>
	inline const T & Stack<T>::top() const
	{
		if (count > 0)
		{
			return pData[count - 1];
		}
		else
		{
			throw std::length_error("the stack is empty");
		}
	}
	template<class T>
	inline void Stack<T>::enlarge()
	{
		size_t new_size = sizeof(T) * (capacity + SIZE_INCR);
		void * ptr = realloc(pData, new_size);
		assert(ptr != nullptr);
		pData = reinterpret_cast<T*>(ptr);
		memset(pData + capacity, 0, sizeof(T) * SIZE_INCR);
		capacity += SIZE_INCR;
	}
}



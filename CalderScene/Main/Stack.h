#pragma once
#include <stdexcept>
#include <memory>
#include <algorithm>

namespace utils
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
		static const size_t SIZE_INCR = 8;

		size_t count;
		size_t capacity;

		void enlarge();

		std::unique_ptr<T[]> pData;
	};
	template<class T>
	inline Stack<T>::Stack(): count(0), capacity(SIZE_INCR),pData(new T[SIZE_INCR])
	{
	}
	template<class T>
	inline Stack<T>::~Stack()
	{
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
		pData[count++] = item;
		if (count == capacity)
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
		size_t new_size = capacity + SIZE_INCR;
		std::unique_ptr<T[]> buf(new T[new_size]);
		std::copy(&pData[0], &pData[0] + capacity, &buf[0]);
		pData = std::move(buf);
		capacity = new_size;
	}
}



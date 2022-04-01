#pragma once

#include <initializer_list>

#include <lev/core/util.h>

namespace lev
{
	template <typename T, u64 Size>
	class Array
	{
	public:
		Array();
        Array(std::initializer_list<T> data);
		Array(const Array& other);
		Array& operator = (const Array& other);
		~Array();

		void fill(const T& value);

		constexpr u64 max_size() const;

		T& at(u64 idx);
		const T& at(u64 idx) const;

		T* data();
		const T* data() const;

        T& operator [] (u64 idx);
        const T& operator [] (u64 idx) const;

	private:
		T m_buf[Size];
	};

	template <typename T, u64 Size>
	Array<T, Size>::Array()
	{
	}
	
	template <typename T, u64 Size>
	Array<T, Size>::Array(std::initializer_list<T> data)
	{
		for (u64 i = 0; i < Size; i++)
			m_buf[i] = data[i];
	}
	
	template <typename T, u64 Size>
	Array<T, Size>::Array(const Array& other)
	{
		for (u64 i = 0; i < Size; i++)
			m_buf[i] = other.m_buf[i];
	}
	
	template <typename T, u64 Size>
	Array<T, Size>& Array<T, Size>::operator = (const Array& other)
	{
		for (u64 i = 0; i < Size; i++)
			m_buf[i] = other.m_buf[i];
	}
	
	template <typename T, u64 Size>
	Array<T, Size>::~Array()
	{
	}

	template <typename T, u64 Size>
	void Array<T, Size>::fill(const T& value)
	{
		for (int i = 0; i < Size; i++)
			m_buf[i] = value;
	}

	template <typename T, u64 Size>
	constexpr u64 Array<T, Size>::max_size() const
	{
		return Size;
	}

	template <typename T, u64 Size>
	T& Array<T, Size>::at(u64 idx)
	{
		return m_buf[idx];
	}

	template <typename T, u64 Size>
	const T& Array<T, Size>::at(u64 idx) const
	{
		return m_buf[idx];
	}

	template <typename T, u64 Size>
	T* Array<T, Size>::data()
	{
		return m_buf;
	}

	template <typename T, u64 Size>
	const T* Array<T, Size>::data() const
	{
		return m_buf;
	}
	
	template <typename T, u64 Size>
    T& Array<T, Size>::operator [] (u64 idx)
	{
		return m_buf[idx];
	}
	
	template <typename T, u64 Size>
    const T& Array<T, Size>::operator [] (u64 idx) const
	{
		return m_buf[idx];
	}
}

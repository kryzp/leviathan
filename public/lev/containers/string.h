#pragma once

#include <lev/core/util.h>

namespace lev
{
	// TODO UNFINISHED

	template <u64 Size>
	class Str
	{
	public:
		Str();
		Str(const char* str);

		Str(const Str& other);
		Str(Str&& other) noexcept;

		Str& operator = (const Str& other);
		Str& operator = (Str&& other) noexcept;

		~Str();

		void clear();

		char* c_str();
		const char* c_str() const;

		u64 length() const;
		constexpr u64 size() const;

		char& operator [] (u64 idx);

		bool operator == (const Str& other) const;
		bool operator != (const Str& other) const;

		operator const char* () const;

	private:
		char m_buf[Size];//{0};
		u64 m_length;
	};

	using String = Str<64>;

	template <u64 Size>
	Str<Size>::Str()
		: m_length(0)
	{
		mem::set(m_buf, 0, m_length);
	}

	template <u64 Size>
	Str<Size>::Str(const char* str)
		: m_length(str::length(str))
	{
		LEV_ASSERT(m_length < (Size - 1), "Length must not exceed maximum size"); // -1 for '\0'
		str::copy(m_buf, str, m_length);
		m_buf[m_length] = '\0';
	}
	
	template <u64 Size>
	Str<Size>::Str(const Str& other)
	{
		LEV_ASSERT(other.m_length < (Size - 1), "Length must not exceed maximum size");

		m_length = other.m_length;
		str::copy(m_buf, other.m_buf, other.m_length);
		m_buf[m_length] = '\0';
	}
	
	template <u64 Size>
	Str<Size>::Str(Str&& other) noexcept
	{
		LEV_ASSERT(other.m_length < (Size - 1), "Length must not exceed maximum size");
		
		if (m_length > other.m_length)
			mem::set(m_buf, 0, m_length);

		m_length = std::move(other.m_length);
		str::copy(m_buf, std::move(other.m_buf), m_length);
		m_buf[m_length] = '\0';

		other.m_length = 0;
		mem::set(other.m_buf, 0, other.size());
	}
	
	template <u64 Size>
	Str<Size>& Str<Size>::operator = (const Str& other)
	{
		LEV_ASSERT(other.m_length < (Size - 1), "Length must not exceed maximum size");
		
		if (m_length > other.m_length)
			mem::set(m_buf, 0, m_length);

		m_length = other.m_length;
		str::copy(m_buf, other.m_buf, other.m_length);
		m_buf[m_length] = '\0';

		return *this;
	}
	
	template <u64 Size>
	Str<Size>& Str<Size>::operator = (Str&& other) noexcept
	{
		LEV_ASSERT(other.m_length < (Size - 1), "Length must not exceed maximum size");
		
		if (m_length > other.m_length)
			mem::set(m_buf, 0, m_length);

		m_length = std::move(other.m_length);
		str::copy(m_buf, std::move(other.m_buf), other.m_length);
		m_buf[m_length] = '\0';

		other.m_length = 0;
		mem::set(other.m_buf, 0, other.size());

		return *this;
	}

	template <u64 Size>
	Str<Size>::~Str()
	{
		m_length = 0;
	}

	template <u64 Size>
	void Str<Size>::clear()
	{
		mem::set(m_buf, 0, m_length);
		m_length = 0;
	}

	template <u64 Size>
	char* Str<Size>::c_str()
	{
		return m_buf;
	}

	template <u64 Size>
	const char* Str<Size>::c_str() const
	{
		return m_buf;
	}

	template <u64 Size>
	u64 Str<Size>::length() const
	{
		return m_length;
	}

	template <u64 Size>
	constexpr u64 Str<Size>::size() const
	{
		return Size;
	}
	
	template <u64 Size>
	char& Str<Size>::operator [] (u64 idx)
	{
		LEV_ASSERT(idx < m_length, "Index must not be more than the length of the string");
		return m_buf[idx];
	}
	
	template <u64 Size>
	bool Str<Size>::operator == (const Str& other) const
	{
		return str::compare(m_buf, other.m_buf) == 0;
	}

	template <u64 Size>
	bool Str<Size>::operator != (const Str& other) const
	{
		return !(*this == other);
	}

	template <u64 Size>
	Str<Size>::operator const char* () const
	{
		return m_buf;
	}
}

namespace std
{
	// std::hash overload
	template <>
	struct hash<lev::String>
	{
		std::size_t operator()(const lev::String& k) const
		{
			std::size_t hash = 7521;
			for (int i = 0; i < k.length(); i++)
				hash = ((hash << 5) + hash) + k[i];
			return hash;
		}
	};
}

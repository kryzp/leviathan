#pragma once

#include <lev/core/util.h>
#include <lev/containers/vector.h>

// note: easier to manage a fixed size string than one which constantly re-allocs

namespace lev
{
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
		bool empty() const;

		Str append(const Str& str) const;

		Str to_upper() const;
		Str to_lower() const;

		Str trim() const;
		Str trim_start() const;
		Str trim_end() const;

		Vector<Str> split(char seperator = ' ') const;

		int index_of(const Str& str) const;

		bool starts_with(const Str& str) const;
		bool ends_with(const Str& str) const;
		bool contains(const Str& str) const;

		void pop_front();
		void pop_back();

		char* c_str();
		const char* c_str() const;

		u64 length() const;
		constexpr u64 size() const;

		char& operator [] (u64 idx);
		char operator [] (u64 idx) const;

		bool operator == (const Str& other) const;
		bool operator != (const Str& other) const;

		Str operator + (const Str& rhs) const;
		Str& operator += (const Str& other);

		explicit operator char* ();
		explicit operator const char* () const;

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
	bool Str<Size>::empty() const
	{
		return str::compare(m_buf, "") == 0;
	}

	template <u64 Size>
	Str<Size> Str<Size>::append(const Str<Size>& str) const
	{
		LEV_ASSERT((m_length + str.m_length) < (Size - 1), "Final length must not exceed maximum size");

		Str result = *this;
		result.m_length += str.length();
		strncat(result.m_buf, str.m_buf, str.length());

		return result;
	}

	template <u64 Size>
	void Str<Size>::pop_front()
	{
		m_length--;
		mem::move(m_buf, m_buf + 1, m_length);
	}

	template <u64 Size>
	void Str<Size>::pop_back()
	{
		m_length--;
		m_buf[m_length] = '\0';
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
	Str<Size> Str<Size>::trim() const
	{
		return trim_start().trim_end();
	}

	template <u64 Size>
	Str<Size> Str<Size>::trim_start() const
	{
		const char* buffer = m_buf;
		while (str::is_space(*buffer)) buffer++;
		int whitespace = buffer - m_buf;

		Str trimmed = *this;
		trimmed.m_length -= whitespace;
		mem::move(trimmed.m_buf, trimmed.m_buf + whitespace, trimmed.m_length);

		return trimmed;
	}

	template <u64 Size>
	Str<Size> Str<Size>::trim_end() const
	{
		const char* buffer = m_buf + str::length(m_buf) - 1;
		while (buffer > m_buf && str::is_space(*buffer)) buffer--;
		int whitespace = buffer - m_buf;

		Str<Size> trimmed = *this;
		trimmed.m_length -= whitespace;
		trimmed.m_buf[trimmed.m_length + whitespace] = '\0';

		return trimmed;
	}

	template <u64 Size>
	Vector<Str<Size>> Str<Size>::split(char seperator) const
	{
		return Vector<Str>(); // todo
	}

	template <u64 Size>
	Str<Size> Str<Size>::to_upper() const
	{
		return Str(); // todo
	}

	template <u64 Size>
	Str<Size> Str<Size>::to_lower() const
	{
		return Str(); // todo
	}

	template <u64 Size>
	int Str<Size>::index_of(const Str& str) const
	{
		return 0; // todo
	}

	template <u64 Size>
	bool Str<Size>::starts_with(const Str& str) const
	{
		return false; // todo
	}

	template <u64 Size>
	bool Str<Size>::ends_with(const Str& str) const
	{
		return false; // todo
	}

	template <u64 Size>
	bool Str<Size>::contains(const Str& str) const
	{
		return false; // todo
	}

	template <u64 Size>
	char& Str<Size>::operator [] (u64 idx)
	{
		LEV_ASSERT(idx < m_length, "Index must not be more than the length of the string");
		return m_buf[idx];
	}

	template <u64 Size>
	char Str<Size>::operator [] (u64 idx) const
	{
		LEV_ASSERT(idx < m_length, "Index must not be more than the length of the string");
		return m_buf[idx];
	}

	template <u64 Size>
	Str<Size> Str<Size>::operator + (const Str<Size>& other) const
	{
		return this->append(other);
	}

	template <u64 Size>
	Str<Size>& Str<Size>::operator += (const Str<Size>& other)
	{
		*this = this->append(other);
		return *this;
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

	template <u64 Size>
	Str<Size>::operator char* ()
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

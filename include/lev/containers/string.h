#pragma once

#include <lev/core/util.h>
#include <lev/math/calc.h>

namespace lev
{
	template <int Size>
	class Str
	{
	public:
		Str();
		Str(const char* str);
		Str(const Str& other);
		~Str();

		void append(const char* str);

		const char* c_str() const;
		int length() const;
		char at(int i) const;
		bool equals(const Str& other) const;

		char* begin();
		const char* begin() const;
		char* end();
		const char* end() const;

		char operator [] (int i) const;

		Str operator + (const char* str) const;
		Str& operator += (const char* str);

		operator const char* () const;

		bool operator == (const char* str) const;
		bool operator != (const char* str) const;

	private:
		char* m_buf;
		int m_length;
	};

	using String = Str<64>;

	template <int Size>
	Str<Size>::Str()
		: m_buf(nullptr)
		, m_length(0)
	{
		m_buf = new char[Size+1];
		MemUtil::set_zero(m_buf, Size);
		m_buf[Size] = '\0';
	}

	template <int Size>
	Str<Size>::Str(const char* str)
		: m_length(strlen(str))
	{
		m_buf = new char[Size+1];
		MemUtil::copy(m_buf, str, Size);
		m_buf[m_length] = '\0';
	}

	template <int Size>
	Str<Size>::Str(const Str& other)
		: m_length(other.m_length)
	{
		m_buf = new char[Size+1];
		MemUtil::copy(m_buf, other.m_buf, Size);
		m_buf[m_length] = '\0';
	}

	template <int Size>
	Str<Size>::~Str()
	{
	}

	template <int Size>
	void Str<Size>::append(const char* str)
	{
		LEV_ASSERT(strlen(str) + m_length <= Size);

		int arglen = strlen(str);

		MemUtil::copy(
			m_buf + m_length,
			str,
			arglen
		);

		m_length += arglen;
		m_buf[m_length] = '\0';
	}

	template <int Size>
	const char* Str<Size>::c_str() const
	{
		return m_buf;
	}

	template <int Size>
	int Str<Size>::length() const
	{
		return m_length;
	}

	template <int Size>
	char Str<Size>::at(int i) const
	{
		return m_buf[i];
	}

	template <int Size>
	bool Str<Size>::equals(const Str& other) const
	{
		return (
			m_length == other.m_length &&
			MemUtil::compare(m_buf, other.m_buf, m_length) == 0
		);
	}

	template <int Size>
	char* Str<Size>::begin()
	{
		return m_buf;
	}

	template <int Size>
	const char* Str<Size>::begin() const
	{
		return m_buf;
	}

	template <int Size>
	char* Str<Size>::end()
	{
		return m_buf + m_length;
	}

	template <int Size>
	const char* Str<Size>::end() const
	{
		return m_buf + m_length;
	}

	template <int Size>
	char Str<Size>::operator [] (int i) const
	{
		return m_buf[i];
	}

	template <int Size>
	Str<Size> Str<Size>::operator + (const char* str) const
	{
		String tmp = *this;
		tmp.append(str);
		return tmp;
	}

	template <int Size>
	Str<Size>::operator const char* () const
	{
		return m_buf;
	}

	template <int Size>
	Str<Size>& Str<Size>::operator += (const char* str)
	{
		append(str);
		return *this;
	}

	template <int Size>
	bool Str<Size>::operator == (const char* str) const
	{
		return this->equals(str);
	}

	template <int Size>
	bool Str<Size>::operator != (const char* str) const
	{
		return !(this->equals(str));
	}
}

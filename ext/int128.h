
// this is bad do not use your house will explode

#if 0
#pragma once

#include <lev/core/util.h>
#include <limits>
#include <iostream>

/*
Heres how it works:
The 128 bits are divided into two 64 bit ints

<---------------------------[ LEFT ]---------------------------> <---------------------------[ RIGHT ]-------------------------->
0000000000000000000000000000000000000000000000000000000000000000 0000000000000000000000000000000000000000000000000000000000000000

Onto which I can then just do simple binary math

NOTE: INDEXING GOES FROM RIGHT TO LEFT
*/

namespace lev::ext
{
	class Int128
	{
	public:
		Int128();
		Int128(const char* number);

		static Int128 add(const Int128& a, const Int128& b);
		static Int128 sub(const Int128& a, const Int128& b);
		static Int128 mul(const Int128& a, const Int128& b);
		static Int128 div(const Int128& a, const Int128& b);

		bool empty();
		void setbit(int idx, u8 bit);
		u8 getbit(int idx) const;

		void print() const;

	private:
		s64 m_left;
		s64 m_right;
	};

	using i128 = Int128;
	using s128 = Int128;

	Int128::Int128()
		: m_left(0)
		, m_right(0)
	{
	}

	Int128::Int128(const char* number)
		: m_left(0)
		, m_right(0)
	{
		// todo: parse into m_left and m_right

		setbit(3, 1);
		setbit(4, 1);
		setbit(5, 1);
	}

	Int128 Int128::add(const Int128& a, const Int128& b)
	{
		Int128 result;

		// wait can i just do addition on m_right and then just check if that has a carryover bit, apply that to m_left and just do m_left addition?
		// that'd be faster surely

		u8 carryover = 0;
		
		for (int i = 0; i < 128; i++)
		{
			u8 abit = a.getbit(i);
			u8 bbit = b.getbit(i);

			u8 finalbit = (abit ^ bbit) | carryover;
			carryover = abit & bbit;

			result.setbit(i, finalbit);
		}

		return result;
	}

	Int128 Int128::sub(const Int128& a, const Int128& b)
	{
		return Int128();
	}

	Int128 Int128::mul(const Int128& a, const Int128& b)
	{
		// repeated addition

		return Int128();
	}

	Int128 Int128::div(const Int128& a, const Int128& b)
	{
		// repeated subtraction

		return Int128();
	}

	bool Int128::empty()
	{
		return m_left == 0 && m_right == 0;
	}

	void Int128::setbit(int idx, u8 bit)
	{
		LEV_ASSERT(idx >= 0 && idx < 128, "Index must be in range [0 -> 128]!");
		LEV_ASSERT(bit == 0 || bit == 1, "Bit must be 0 or 1!");

		if (idx >= 0 && idx < 64)
		{
			// change right side
			m_right |= bit << idx;
		}
		else if (idx >= 64 && idx < 128)
		{
			// change left size
			m_left |= bit << idx;
		}
	}

	u8 Int128::getbit(int idx) const
	{
		LEV_ASSERT(idx >= 0 && idx < 128, "Index must be in range [0 -> 128]!");

		if (idx >= 0 && idx < 64)
		{
			// return right side
			return (m_right & (1LL << idx)) >> idx;
		}
		else if (idx >= 64 && idx < 128)
		{
			// return left side
			return (m_left & (1LL << idx)) >> idx;
		}
	}

	void Int128::print() const
	{
		std::cout << "lol no" << std::endl;
	}
}
#endif

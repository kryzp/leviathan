#pragma once

#include <lev/core/util.h>

namespace lev
{
	template <u64 Size>
	class Bitset
	{
	public:
		Bitset();
		~Bitset();

		bool all() const;
		bool none() const;
		bool any() const;

		u64 on_count() const;
		u64 off_count() const;

		Bitset& reset();
		Bitset& invert();

		Bitset& enable(u64 idx);
		Bitset& disable(u64 idx);
		Bitset& set(u64 idx, bool mode);
		Bitset& toggle(u64 idx);

		bool on(u64 idx) const;
		bool on_only(u64 idx) const;
		bool off(u64 idx) const;

		bool operator [] (u64 idx) const;
		
		constexpr inline u64 size() const;
		constexpr inline u64 memory_size() const;

	private:
		u8* m_bits;
	};

	template <u64 Size>
	Bitset<Size>::Bitset()
	{
		m_bits = new u8[memory_size()];
		reset();
	}

	template <u64 Size>
	Bitset<Size>::~Bitset()
	{
		delete[] m_bits;
	}

	template <u64 Size>
	bool Bitset<Size>::all() const
	{
		for (u64 i = 0; i < Size; i++)
		{
			if (off(i))
				return false;
		}

		return true;
	}

	template <u64 Size>
	bool Bitset<Size>::none() const
	{
		for (u64 i = 0; i < Size; i++)
		{
			if (on(i))
				return false;
		}

		return true;
	}

	template <u64 Size>
	bool Bitset<Size>::any() const
	{
		return !none();
	}

	template <u64 Size>
	u64 Bitset<Size>::on_count() const
	{
		// popcount???

		u64 total = 0;
		for (u64 i = 0; i < Size; i++)
		{
			if (on(i))
				total++;
		}

		return total;
	}

	template <u64 Size>
	u64 Bitset<Size>::off_count() const
	{
		return Size - on_count();
	}

	template <u64 Size>
	Bitset<Size>& Bitset<Size>::reset()
	{
		mem::set(m_bits, 0, memory_size());
		return *this;
	}

	template <u64 Size>
	Bitset<Size>& Bitset<Size>::invert()
	{
		for (int i = 0; i < Size; i++) toggle(i);
		return *this;
	}

	template <u64 Size>
	Bitset<Size>& Bitset<Size>::enable(u64 idx)
	{
		LEV_ASSERT(idx >= 0 && idx < Size, "Index must be within range of the bitset");
		m_bits[idx/8] |= (1 << idx);
		return *this;
	}

	template <u64 Size>
	Bitset<Size>& Bitset<Size>::disable(u64 idx)
	{
		LEV_ASSERT(idx >= 0 && idx < Size, "Index must be within range of the bitset");
		m_bits[idx/8] &= ~(1 << idx);
		return *this;
	}

	template <u64 Size>
	Bitset<Size>& Bitset<Size>::toggle(u64 idx)
	{
		LEV_ASSERT(idx >= 0 && idx < Size, "Index must be within range of the bitset");
		m_bits[idx/8] ^= (1 << idx);
		return *this;
	}

	template <u64 Size>
	Bitset<Size>& Bitset<Size>::set(u64 idx, bool mode)
	{
		LEV_ASSERT(idx >= 0 && idx < Size, "Index must be within range of the bitset");
		if (mode) m_bits[idx/8] |= (1 << idx);
		else m_bits[idx/8] &= ~(1 << idx);
		return *this;
	}

	template <u64 Size>
	bool Bitset<Size>::on(u64 idx) const
	{
		LEV_ASSERT(idx >= 0 && idx < Size, "Index must be within range of the bitset");
		return (m_bits[idx/8] & (1 << idx)) != 0;
	}

	template <u64 Size>
	bool Bitset<Size>::on_only(u64 idx) const
	{
		LEV_ASSERT(idx >= 0 && idx < Size, "Index must be within range of the bitset");
		return (m_bits[idx/8] & (1 << idx)) == idx;
	}

	template <u64 Size>
	bool Bitset<Size>::off(u64 idx) const
	{
		return !on(idx);
	}

	template <u64 Size>
	bool Bitset<Size>::operator [] (u64 idx) const
	{
		return on(idx);
	}

	template <u64 Size>
	constexpr inline u64 Bitset<Size>::size() const
	{
		return Size;
	}

	template <u64 Size>
	constexpr inline u64 Bitset<Size>::memory_size() const
	{
		return (Size / 8) + 1;
	}
}

#pragma once

namespace lv
{
	template <typename First, typename Second>
	struct Pair
	{
		First first;
		Second second;

		Pair();
		Pair(const First& a, const Second& b);

		Pair(const Pair& other);
		Pair(Pair&& other) noexcept;

		Pair& operator = (const Pair& other);
		Pair& operator = (Pair&& other) noexcept;

		~Pair() = default;

		bool operator == (const Pair& other) const;
		bool operator != (const Pair& other) const;
	};

	template <typename First, typename Second>
	Pair<First, Second>::Pair()
		: first()
		, second()
	{
	}

	template <typename First, typename Second>
	Pair<First, Second>::Pair(const First& a, const Second& b)
		: first(a)
		, second(b)
	{
	}

	template <typename First, typename Second>
	Pair<First, Second>::Pair(const Pair& other)
		: first(other.first)
		, second(other.second)
	{
	}

	template <typename First, typename Second>
	Pair<First, Second>::Pair(Pair&& other) noexcept
		: first(std::move(other.first))
		, second(std::move(other.second))
	{
	}

	template <typename First, typename Second>
	Pair<First, Second>& Pair<First, Second>::operator = (const Pair& other)
	{
		this->first = other.first;
		this->second = other.second;

		return *this;
	}

	template <typename First, typename Second>
	Pair<First, Second>& Pair<First, Second>::operator = (Pair&& other) noexcept
	{
		this->first = std::move(other.first);
		this->second = std::move(other.second);

		return *this;
	}

	template <typename First, typename Second>
	bool Pair<First, Second>::operator == (const Pair& other) const
	{
		return (
			this->first == other.first &&
			this->second == other.second
		);
	}

	template <typename First, typename Second>
	bool Pair<First, Second>::operator != (const Pair& other) const
	{
		return !(*this == other);
	}
}

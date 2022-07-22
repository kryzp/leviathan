#pragma once

#include <lev/core/util.h>

#include <random>
#include <ctime>

namespace lev
{
	template <typename TEngine = std::mt19937>
	class Rand
	{
	public:
		static Rand* inst()
		{
			static Rand* instance = nullptr;
			if (!instance) { instance = new Rand(); }
			return instance;
		}

		Rand();

		int integer(int min, int max);
		float real(float min, float max);

		template <typename TDist, typename T>
		T generic_range(T min, T max);

	private:
		TEngine m_rng;
	};

	template <typename TEngine>
	Rand<TEngine>::Rand()
		: m_rng(std::time(nullptr))
	{
	}

	template <typename TEngine>
	int Rand<TEngine>::integer(int min, int max)
	{
		return generic_range<std::uniform_int_distribution<int>>(min, max);
	}

	template <typename TEngine>
	float Rand<TEngine>::real(float min, float max)
	{
		return generic_range<std::uniform_real_distribution<float>>(min, max);
	}

	template <typename TEngine>
	template <typename TDist, typename T>
	T Rand<TEngine>::generic_range(T min, T max)
	{
		TDist dist = TDist(min, max);
		return dist(m_rng);
	}
}

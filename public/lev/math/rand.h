#pragma once

#include <lev/core/util.h>

#include <random>
#include <ctime>

namespace lv
{
	template <typename Engine = std::mt19937>
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

		template <typename Dist, typename T>
		T generic_range(T min, T max);

	private:
		Engine m_rng;
	};

	template <typename Engine>
	Rand<Engine>::Rand()
		: m_rng(std::time(nullptr))
	{
	}

	template <typename Engine>
	int Rand<Engine>::integer(int min, int max)
	{
		return generic_range<std::uniform_int_distribution<int>>(min, max);
	}

	template <typename Engine>
	float Rand<Engine>::real(float min, float max)
	{
		return generic_range<std::uniform_real_distribution<float>>(min, max);
	}

	template <typename Engine>
	template <typename Dist, typename T>
	T Rand<Engine>::generic_range(T min, T max)
	{
		Dist dist = Dist(min, max);
		return dist(m_rng);
	}
}

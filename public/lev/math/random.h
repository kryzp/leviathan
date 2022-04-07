#pragma once

#include <lev/core/util.h>

#include <random>
#include <ctime>

namespace lev
{
	template <typename Engine = std::mt19937>
	class Random
	{
		LEV_SINGLETON_CLASS(Random);

	public:
		Random();

		int integer(int min, int max);
		float real(float min, float max);

		template <typename Dist, typename T>
		T generic_range(T min, T max);

	private:
		Engine m_rng;
	};

	template <typename Engine>
	Random<Engine>::Random()
		: m_rng(std::time(nullptr))
	{
	}

	template <typename Engine>
	int Random<Engine>::integer(int min, int max)
	{
		return generic_range<std::uniform_int_distribution<int>>(min, max);
	}

	template <typename Engine>
	float Random<Engine>::real(float min, float max)
	{
		return generic_range<std::uniform_real_distribution<float>>(min, max);
	}

	template <typename Engine>
	template <typename Dist, typename T>
	T Random<Engine>::generic_range(T min, T max)
	{
		Dist dist = Dist(min, max);
		return dist(m_rng);
	}
}

#pragma once

#include <random>
#include <ctime>

namespace Lev
{
	template <typename Engine = std::mt19937>
	class Rng
	{
	public:
		Rng();

		int integer(int min, int max);
		float real(float min, float max);

		template <typename Dist, typename T>
		T generic_range(T min, T max);

	private:
		Engine m_rng;

	};

	template <typename Engine>
	Rng<Engine>::Rng()
		: m_rng(std::time(nullptr))
	{
	}

	template <typename Engine>
	int Rng<Engine>::integer(int min, int max)
	{
		return generic_range<std::uniform_int_distribution<int>>(min, max);
	}

	template <typename Engine>
	float Rng<Engine>::real(float min, float max)
	{
		return generic_range<std::uniform_real_distribution<float>>(min, max);
	}

	template <typename Engine>
	template <typename Dist, typename T>
	T Rng<Engine>::generic_range(T min, T max)
	{
		Dist dist = Dist(min, max);
		return dist(m_rng);
	}
}

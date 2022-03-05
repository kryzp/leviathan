#pragma once

#include <lev/math/vec2.h>

namespace lev
{
	template <typename T>
	struct Pair
	{
		static const Pair ZERO;
		static const Pair ONE;

		T x;
		T y;

		Pair();
		Pair(float x);
		Pair(float x, float y);

		Vec2 to_vec2() const;

		Pair operator + (const Pair& other) const;
		Pair operator - (const Pair& other) const;
		Pair operator - () const;
		Pair operator * (const Pair& other) const;
		Pair operator / (const Pair& other) const;

		Pair& operator += (const Pair& other);
		Pair& operator -= (const Pair& other);
		Pair& operator *= (const Pair& other);
		Pair& operator /= (const Pair& other);
	};

	template <typename T> const Pair<T> Pair<T>::ZERO = Pair<T>(0, 0);
	template <typename T> const Pair<T> Pair<T>::ONE  = Pair<T>(1, 1);

	using Float2 = Pair<float>;
	using Int2   = Pair<float>;
	using Bool2  = Pair<float>;

	template <typename T>
	Pair<T>::Pair()
		: x(0)
		, y(0)
	{
	}

	template <typename T>
	Pair<T>::Pair(float x)
		: x(x)
		, y(x)
	{
	}

	template <typename T>
	Pair<T>::Pair(float x, float y)
		: x(x)
		, y(y)
	{
	}

	template <typename T> Vec2 Pair<T>::to_vec2() const { return Vec2(x, y); }

	template <typename T> Pair<T> Pair<T>::operator + (const Pair<T>& other) const { return Pair<T>( this->x + other.x,  this->y + other.y); }
	template <typename T> Pair<T> Pair<T>::operator - (const Pair<T>& other) const { return Pair<T>( this->x - other.x,  this->y - other.y); }
	template <typename T> Pair<T> Pair<T>::operator - ()					 const { return Pair<T>(-this->x,           -this->y); }
	template <typename T> Pair<T> Pair<T>::operator * (const Pair<T>& other) const { return Pair<T>( this->x * other.x,  this->y * other.y); }
	template <typename T> Pair<T> Pair<T>::operator / (const Pair<T>& other) const { return Pair<T>( this->x / other.x,  this->y / other.y); }

	template <typename T> Pair<T>& Pair<T>::operator += (const Pair<T>& other) { this->x += other.x; this->y += other.y; return *this; }
	template <typename T> Pair<T>& Pair<T>::operator -= (const Pair<T>& other) { this->x -= other.x; this->y -= other.y; return *this; }
	template <typename T> Pair<T>& Pair<T>::operator *= (const Pair<T>& other) { this->x *= other.x; this->y *= other.y; return *this; }
	template <typename T> Pair<T>& Pair<T>::operator /= (const Pair<T>& other) { this->x /= other.x; this->y /= other.y; return *this; }
}

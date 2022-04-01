#pragma once

#include <lev/math/calc.h>
#include <lev/math/mat3x2.h>

namespace lev
{
	template <typename T>
	struct Vec2
	{
		union
		{
			struct
			{
				T x;
				T y;
			};

			struct
			{
				T a;
				T b;
			};

			struct
			{
				T first;
				T second;
			};

			struct
			{
				T w;
				T h;
			};

			struct
			{
				T width;
				T height;
			};

			T coords[2];
			T data[2];
		};

		Vec2();
		Vec2(T x);
		Vec2(T x, T y);
		
		template <typename Y>
		Vec2(const Vec2<Y>& other)
			: x(other.x)
			, y(other.y)
		{
		}

		static const Vec2& zero();
		static const Vec2& one();
		static const Vec2& left();
		static const Vec2& right();
		static const Vec2& up();
		static const Vec2& down();

		static Vec2 transform(const Vec2& vec, const Mat3x2& mat);
		static float dot(const Vec2& a, const Vec2& b);
		static Vec2 from_angle(float angle, float length);
		static Vec2 lerp(const Vec2& from, const Vec2& to, float amount);
		static Vec2 spring(const Vec2& from, const Vec2& to, float bounciness, float tension, Vec2& intermediate);

		float angle() const;

		float length() const;
		float length_squared() const;

		Vec2 normalized() const;
		Vec2 perpendicular() const;

		bool operator == (const Vec2& other) const;
		bool operator != (const Vec2& other) const;

		Vec2 operator + (const Vec2& other) const;
		Vec2 operator - (const Vec2& other) const;
		Vec2 operator * (const Vec2& other) const;
		Vec2 operator / (const Vec2& other) const;

		Vec2 operator - () const;

		Vec2& operator += (const Vec2& other);
		Vec2& operator -= (const Vec2& other);
		Vec2& operator *= (const Vec2& other);
		Vec2& operator /= (const Vec2& other);
	};

	using Vec2F		= Vec2<float>;
	using Vec2I		= Vec2<int>;

	using Float2	= Vec2<float>;
	using Size2		= Vec2<float>;
	using Int2		= Vec2<int>;
	using Point2	= Vec2<int>;

	template <typename T>
	Vec2<T>::Vec2()
		: x(0)
		, y(0)
	{
	}
	
	template <typename T>
	Vec2<T>::Vec2(T x)
		: x(x)
		, y(x)
	{
	}
	
	template <typename T>
	Vec2<T>::Vec2(T x, T y)
		: x(x)
		, y(y)
	{
	}
	
	template <typename T>
	Vec2<T> Vec2<T>::transform(const Vec2& vec, const Mat3x2& mat)
	{
		return Vec2(
			(vec.x * mat.m11) + (vec.y * mat.m21) + mat.m31,
			(vec.x * mat.m12) + (vec.y * mat.m22) + mat.m32
		);
	}
	
	template <typename T>
	float Vec2<T>::dot(const Vec2& a, const Vec2& b)
	{
		return a.x*b.x + a.y*b.y;
	}
	
	template <typename T>
	Vec2<T> Vec2<T>::from_angle(float angle, float length)
	{
		return Vec2(
			Calc::cos(angle) * length,
			Calc::sin(angle) * length
		);
	}
	
	template <typename T>
	Vec2<T> Vec2<T>::lerp(const Vec2& from, const Vec2& to, float amount)
	{
		return Vec2(
			Calc::lerp(from.x, to.x, amount),
			Calc::lerp(from.y, to.y, amount)
		);
	}
	
	template <typename T>
	Vec2<T> Vec2<T>::spring(const Vec2& from, const Vec2& to, float bounciness, float tension, Vec2& intermediate)
	{
		intermediate.x = Calc::spring(from.x, to.x, bounciness, tension, intermediate.x);
		intermediate.y = Calc::spring(from.y, to.y, bounciness, tension, intermediate.y);
		return intermediate;
	}
	
	template <typename T>
	float Vec2<T>::angle() const
	{
		return Calc::atan2(y, x);
	}
	
	template <typename T>
	float Vec2<T>::length() const
	{
		return Calc::sqrt(length_squared());
	}
	
	template <typename T>
	float Vec2<T>::length_squared() const
	{
		return x*x + y*y;
	}
	
	template <typename T>
	Vec2<T> Vec2<T>::normalized() const
	{
		float len = length();

		return Vec2(
			x / len,
			y / len
		);
	}
	
	template <typename T>
	Vec2<T> Vec2<T>::perpendicular() const
	{
		return Vec2(-y, x);
	}

	template <typename T> bool Vec2<T>::operator == (const Vec2& other) const { return this->x == other.x && this->y == other.y; }
	template <typename T> bool Vec2<T>::operator != (const Vec2& other) const { return !(*this == other); }

	template <typename T> Vec2<T> Vec2<T>::operator + (const Vec2& other) const { return Vec2( this->x + other.x,  this->y + other.y); }
	template <typename T> Vec2<T> Vec2<T>::operator - (const Vec2& other) const { return Vec2( this->x - other.x,  this->y - other.y); }
	template <typename T> Vec2<T> Vec2<T>::operator - ()                  const { return Vec2(-this->x,           -this->y          ); }
	template <typename T> Vec2<T> Vec2<T>::operator * (const Vec2& other) const { return Vec2( this->x * other.x,  this->y * other.y); }
	template <typename T> Vec2<T> Vec2<T>::operator / (const Vec2& other) const { return Vec2( this->x / other.x,  this->y / other.y); }

	template <typename T> Vec2<T>& Vec2<T>::operator += (const Vec2& other) { this->x += other.x; this->y += other.y; return *this; }
	template <typename T> Vec2<T>& Vec2<T>::operator -= (const Vec2& other) { this->x -= other.x; this->y -= other.y; return *this; }
	template <typename T> Vec2<T>& Vec2<T>::operator *= (const Vec2& other) { this->x *= other.x; this->y *= other.y; return *this; }
	template <typename T> Vec2<T>& Vec2<T>::operator /= (const Vec2& other) { this->x /= other.x; this->y /= other.y; return *this; }

	template <typename T> const Vec2<T>& Vec2<T>::zero()	{ static const Vec2 ZERO	= Vec2( 0,  0); return ZERO;	}
	template <typename T> const Vec2<T>& Vec2<T>::one()		{ static const Vec2 ONE		= Vec2( 1,  1); return ONE;		}
	template <typename T> const Vec2<T>& Vec2<T>::left()	{ static const Vec2 LEFT	= Vec2(-1,  0); return LEFT;	}
	template <typename T> const Vec2<T>& Vec2<T>::right()	{ static const Vec2 RIGHT	= Vec2( 1,  0); return RIGHT;	}
	template <typename T> const Vec2<T>& Vec2<T>::up()		{ static const Vec2 UP		= Vec2( 0, -1); return UP;		}
	template <typename T> const Vec2<T>& Vec2<T>::down()	{ static const Vec2 DOWN	= Vec2( 0,  1); return DOWN;	}
}
#pragma once

#include <lev/math/calc.h>

namespace lev
{
	template <typename T>
	struct Vec3
	{
		union
		{
			struct
			{
				T x;
				T y;
				T z;
			};

			struct
			{
				T a;
				T b;
				T c;
			};

			struct
			{
				T first;
				T second;
				T third;
			};

			struct
			{
				T width;
				T height;
				T depth;
			};

			struct
			{
				T w;
				T h;
				T d;
			};

			T coords[3];
			T data[3];
		};

		Vec3();
		Vec3(T x);
		Vec3(T x, T y, T z);

		template <typename Y>
		Vec3(const Vec3<Y>& other)
			: x(other.x)
			, y(other.y)
			, z(other.z)
		{
		}

		static const Vec3& zero();
		static const Vec3& one();
		static const Vec3& left();
		static const Vec3& right();
		static const Vec3& up();
		static const Vec3& down();
		static const Vec3& forward();
		static const Vec3& backward();

		static Vec3 cross(const Vec3& a, const Vec3& b);
		static Vec3 lerp(const Vec3& from, const Vec3& to, float amount);
		static Vec3 spring(const Vec3& from, const Vec3& to, float bounciness, float tension, Vec3& intermediate);

		float length() const;
		float length_squared() const;

		Vec3 normalized() const;

		bool operator == (const Vec3& other) const;
		bool operator != (const Vec3& other) const;

		Vec3<T> operator + (const Vec3& other) const;
		Vec3<T> operator - (const Vec3& other) const;
		Vec3<T> operator * (const Vec3& other) const;
		Vec3<T> operator / (const Vec3& other) const;

		Vec3<T> operator - () const;

		Vec3<T>& operator += (const Vec3& other);
		Vec3<T>& operator -= (const Vec3& other);
		Vec3<T>& operator *= (const Vec3& other);
		Vec3<T>& operator /= (const Vec3& other);
	};

	using Vec3F		= Vec3<float>;
	using Vec3I		= Vec3<int>;

	using Float3	= Vec3<float>;
	using Size3		= Vec3<float>;
	using Int3		= Vec3<int>;
	using Point3	= Vec3<int>;
	
	template <typename T>
	Vec3<T>::Vec3()
		: x(0)
		, y(0)
		, z(0)
	{
	}

	template <typename T>
	Vec3<T>::Vec3(T x)
		: x(x)
		, y(x)
		, z(x)
	{
	}

	template <typename T>
	Vec3<T>::Vec3(T x, T y, T z)
		: x(x)
		, y(y)
		, z(z)
	{
	}

	template <typename T>
	Vec3<T> Vec3<T>::cross(const Vec3<T>& a, const Vec3<T>& b)
	{
	}
	
	template <typename T>
	Vec3<T> Vec3<T>::lerp(const Vec3& from, const Vec3& to, float amount)
	{
		return Vec3(
			Calc::lerp(from.x, to.x, amount),
			Calc::lerp(from.y, to.y, amount),
			Calc::lerp(from.z, to.z, amount)
		);
	}

	template <typename T>
	Vec3<T> Vec3<T>::spring(const Vec3& from, const Vec3& to, float bounciness, float tension, Vec3& intermediate)
	{
		intermediate.x = Calc::spring(from.x, to.x, bounciness, tension, intermediate.x);
		intermediate.y = Calc::spring(from.y, to.y, bounciness, tension, intermediate.y);
		intermediate.z = Calc::spring(from.z, to.z, bounciness, tension, intermediate.z);
		return intermediate;
	}

	template <typename T>
	float Vec3<T>::length() const
	{
		return Calc::sqrt(length_squared());
	}

	template <typename T>
	float Vec3<T>::length_squared() const
	{
		return x*x + y*y + z*z;
	}

	template <typename T>
	Vec3<T> Vec3<T>::normalized() const
	{
		float len = length();

		return Vec3(
			x / len,
			y / len,
			z / len
		);
	}

	template <typename T> bool Vec3<T>::operator == (const Vec3& other) const { return this->x == other.x && this->y == other.y && this->z == other.z; }
	template <typename T> bool Vec3<T>::operator != (const Vec3& other) const { return !(*this == other); }

	template <typename T> Vec3<T> Vec3<T>::operator + (const Vec3& other) const { return Vec3(this->x + other.x, this->y + other.y, this->z + other.z); }
	template <typename T> Vec3<T> Vec3<T>::operator - (const Vec3& other) const { return Vec3(this->x - other.x, this->y - other.y, this->z - other.z); }
	template <typename T> Vec3<T> Vec3<T>::operator * (const Vec3& other) const { return Vec3(this->x * other.x, this->y * other.y, this->z * other.z); }
	template <typename T> Vec3<T> Vec3<T>::operator / (const Vec3& other) const { return Vec3(this->x / other.x, this->y / other.y, this->z / other.z); }

	template <typename T> Vec3<T> Vec3<T>::operator - () const { return Vec3(-this->x, -this->y, -this->z); }

	template <typename T> Vec3<T>& Vec3<T>::operator += (const Vec3& other) { this->x += other.x; this->y += other.y; this->z += other.z; return *this; }
	template <typename T> Vec3<T>& Vec3<T>::operator -= (const Vec3& other) { this->x -= other.x; this->y -= other.y; this->z -= other.z; return *this; }
	template <typename T> Vec3<T>& Vec3<T>::operator *= (const Vec3& other) { this->x *= other.x; this->y *= other.y; this->z *= other.z; return *this; }
	template <typename T> Vec3<T>& Vec3<T>::operator /= (const Vec3& other) { this->x /= other.x; this->y /= other.y; this->z /= other.z; return *this; }

	template <typename T> const Vec3<T>& Vec3<T>::zero()		{ static const Vec3 ZERO		= Vec3( 0,  0,  0); return ZERO;		}
	template <typename T> const Vec3<T>& Vec3<T>::one()			{ static const Vec3 ONE			= Vec3( 1,  1,  1); return ONE;			}
	template <typename T> const Vec3<T>& Vec3<T>::left()		{ static const Vec3 LEFT		= Vec3(-1,  0,  0); return LEFT;		}
	template <typename T> const Vec3<T>& Vec3<T>::right()		{ static const Vec3 RIGHT		= Vec3( 1,  0,  0); return RIGHT;		}
	template <typename T> const Vec3<T>& Vec3<T>::up()			{ static const Vec3 UP			= Vec3( 0, -1,  0); return UP;			} // todo: should vec3's have their y axis go up as y increases? (opposite of vec2)
	template <typename T> const Vec3<T>& Vec3<T>::down()		{ static const Vec3 DOWN		= Vec3( 0,  1,  0); return DOWN;		}
	template <typename T> const Vec3<T>& Vec3<T>::forward()		{ static const Vec3 FORWARD		= Vec3( 0,  0,  1); return FORWARD;		}
	template <typename T> const Vec3<T>& Vec3<T>::backward()	{ static const Vec3 BACKWARD	= Vec3( 0,  0, -1); return BACKWARD;	}
}
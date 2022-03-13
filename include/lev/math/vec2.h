#pragma once

#include <lev/math/mat3x2.h>
#include <iostream>

namespace lev
{
	struct Vec2
	{
		union
		{
			struct
			{
				float x;
				float y;
			};

			struct
			{
				union
				{
					float w;
					float width;
				};

				union
				{
					float h;
					float height;
				};
			};

			float coords[2];
		};

		Vec2();
		Vec2(float x);
		Vec2(float x, float y);

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
		static Vec2 spring(const Vec2& from, const Vec2& to, float amount, float dampening, Vec2& intermediate);

		float angle() const;

		float length() const;
		float length_squared() const;

		Vec2 normalized() const;
		Vec2 perpendicular() const;

		bool operator == (const Vec2& other) const;
		bool operator != (const Vec2& other) const;

		Vec2 operator + (const Vec2& other) const;
		Vec2 operator - (const Vec2& other) const;
		Vec2 operator - () const;
		Vec2 operator * (const Vec2& other) const;
		Vec2 operator / (const Vec2& other) const;

		Vec2& operator += (const Vec2& other);
		Vec2& operator -= (const Vec2& other);
		Vec2& operator *= (const Vec2& other);
		Vec2& operator /= (const Vec2& other);

		friend std::ostream& operator << (std::ostream& os, const Vec2& v);
	};

	using Point2 = Vec2;
	using Float2 = Vec2;
	using Size2 = Vec2;

	inline std::ostream& operator << (std::ostream& os, const Vec2& v)
	{
		std::cout << "{ " << v.x << ", " << v.y << " }" << std::endl;
		return os;
	}
}

#pragma once

namespace LEV
{
	struct Vec2
	{
		float x;
		float y;

		Vec2()
			: x(0)
			, y(0)
		{
		}

		Vec2(float x)
			: x(x)
			, y(x)
		{
		}

		Vec2(float x, float y)
			: x(x)
			, y(y)
		{
		}

		float length() const;
		float length_squared() const;

		Vec2 normalized() const;

		static float dot(const Vec2& a, const Vec2& b);
	};
}

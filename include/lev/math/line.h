#pragma once

#include <lev/math/vec2.h>

namespace lev
{
	struct Line
	{
		Vec2 a;
		Vec2 b;

		Line();
		Line(const Vec2& a, const Vec2& b);
		Line(const Vec2& pos, float angle, float len);

		float length() const;
		float length_squared() const;
	};
}

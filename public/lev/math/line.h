#pragma once

#include <lev/math/vec2.h>

namespace lev
{
	struct Line
	{
		Vec2F a;
		Vec2F b;

		Line();
		Line(const Vec2F& a, const Vec2F& b);
		Line(const Vec2F& pos, float angle, float len);

		Vec2F delta() const;
		Vec2F direction() const;
		float length() const;
		float length_squared() const;
		float angle() const;
	};
}

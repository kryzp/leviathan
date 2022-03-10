#pragma once

#include <lev/math/vec2.h>

namespace lev
{
	struct Triangle
	{
		Vec2 a;
		Vec2 b;
		Vec2 c;

		Triangle();
		Triangle(const Vec2& a, const Vec2& b, const Vec2& c);
	};
}

#pragma once

#include <lev/math/vec2.h>

namespace Lev
{
	struct Rect;

	struct Quad
	{
		Vec2 a;
		Vec2 b;
		Vec2 c;
		Vec2 d;

		Quad();
		Quad(const Vec2& a, const Vec2& b, const Vec2& c, const Vec2& d);
		Quad(const Rect& rect);
	};
}

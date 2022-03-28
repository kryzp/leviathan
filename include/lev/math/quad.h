#pragma once

#include <lev/math/vec2.h>
#include <lev/math/rect.h>

namespace lev
{
	struct Quad
	{
		Vec2F a;
		Vec2F b;
		Vec2F c;
		Vec2F d;

		Quad();
		Quad(const Vec2F& a, const Vec2F& b, const Vec2F& c, const Vec2F& d);
		Quad(const RectF& rect);
	};
}

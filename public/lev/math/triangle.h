#pragma once

#include <lev/math/vec2.h>

namespace lev
{
	struct Triangle
	{
		Vec2F a;
		Vec2F b;
		Vec2F c;

		Triangle();
		Triangle(const Vec2F& a, const Vec2F& b, const Vec2F& c);

		Vec2F centre() const;
	};
}

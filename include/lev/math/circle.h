#pragma once

#include <lev/math/vec2.h>

namespace lev
{
	struct Circle
	{
		static const Circle ZERO;
		static const Circle ONE;

		Vec2 position;
		float radius;

		Circle();
		Circle(float rad);
		Circle(const Vec2& pos, float rad);

		float diameter() const;
	};
}

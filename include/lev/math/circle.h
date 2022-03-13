#pragma once

#include <lev/math/vec2.h>

namespace lev
{
	struct Circle
	{
		Vec2 position;
		float radius;

		Circle();
		Circle(float rad);
		Circle(const Vec2& pos, float rad);

		static const Circle& zero();
		static const Circle& one();

		float diameter() const;
	};
}

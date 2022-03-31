#pragma once

#include <lev/math/vec2.h>

namespace lev
{
	struct Circle
	{
		Vec2F position;
		float radius;

		Circle();
		Circle(float rad);
		Circle(const Vec2F& pos, float rad);

		static const Circle& zero();
		static const Circle& one();

		float diameter() const;
	};
}

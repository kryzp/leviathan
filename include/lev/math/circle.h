#pragma once

#include <lev/math/vec2.h>

namespace Lev
{
	struct Circle
	{
		Vec2 position;
		float radius;

		Circle();
		Circle(float rad);
		Circle(const Vec2& pos, float rad);

		float diameter() const;
	};
}

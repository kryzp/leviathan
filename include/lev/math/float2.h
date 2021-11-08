#pragma once

namespace Lev
{
	struct Vec2;

	struct Float2
	{
		float x;
		float y;

		Float2();
		Float2(float x);
		Float2(float x, float y);

		Vec2 to_vec2() const;
	};
}

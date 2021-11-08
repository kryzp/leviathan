#pragma once

namespace Lev
{
	struct Vec2;

	struct Rect
	{
		float x;
		float y;
		float w;
		float h;

		Rect();
		Rect(float w, float h);
		Rect(float x, float y, float w, float h);

		float left() const;
		float right() const;
		float top() const;
		float bottom() const;

		bool intersects(const Vec2& other);
		bool intersects(const Rect& other);
	};
}

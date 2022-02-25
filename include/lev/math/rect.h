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

		Vec2 top_left() const;
		Vec2 top_right() const;
		Vec2 bottom_left() const;
		Vec2 bottom_right() const;

		bool contains(const Vec2& other) const;
		bool intersects(const Rect& other) const;
	};
}

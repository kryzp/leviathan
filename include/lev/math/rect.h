#pragma once

#include <iostream>

namespace Lev
{
	struct Vec2;

	struct Rect
	{
		static const Rect ZERO;
		static const Rect ONE;

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

		friend std::ostream& operator << (std::ostream& os, const Rect& r);
	};

	inline std::ostream& operator << (std::ostream& os, const Rect& r)
	{
		std::cout << "{ " << r.x << ", " << r.y << ", " << r.w << ", " << r.h << " }" << std::endl;
		return os;
	}
}

#pragma once

#include <lev/containers/vector.h>
#include <functional>

namespace lev
{
	struct Vec2;

	struct Polygon
	{
		Vector<Vec2> vertices;
		
		Polygon();
		Polygon(const Vector<Vec2>& vs);

		static bool axis_overlaps(const Polygon& a, const Polygon& b, const Vec2& axis, float* amount);

		void project(const Vec2& axis, float* min, float* max) const;

		//void foreach_point(void (*on_vertex)(int, const Vec2&, const Vec2&));
		void foreach_point(const std::function<void(int, const Vec2&, const Vec2&)>& on_vertex);
	};
}

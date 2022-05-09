#pragma once

#include <lev/containers/vector.h>
#include <functional>
#include <lev/math/vec2.h>

namespace lv
{
	struct Polygon
	{
		Vector<Vec2F> vertices;
		
		Polygon();
		Polygon(const Vector<Vec2F>& vs);

		static bool axis_overlaps(const Polygon& a, const Polygon& b, const Vec2F& axis, float* amount);

		void project(const Vec2F& axis, float* min, float* max) const;
		void foreach_point(const std::function<void(int, Vec2F&, Vec2F&)>& fn);
	};
}

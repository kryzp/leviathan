#pragma once

#include <lev/math/vec2.h>
#include <lev/math/polygon.h>
#include <lev/math/rect.h>
#include <lev/math/transform.h>
#include <lev/containers/vector.h>

namespace Lev
{
	class Collider;

	struct Hit
	{
		Collider* other;
		Vec2 pushout;
		bool solid;
	};

	class Collider
	{
	public:
		Collider();
		Collider(const Collider& other);

		Collider(const Polygon& poly);
		Collider(const Rect& rect);
		Collider(float x, float y, float w, float h);
		
		void make_polygon(const Polygon& polygon);
		void make_rect(const Rect& rect);

		Collider get_offset(const Vec2& offset) const;

		Rect get_world_bounds();

		bool overlaps(Collider& other, Vec2* pushout);

		Transform transform;
		Polygon polygon;
		Polygon world_polygon;

	private:
		static bool poly_to_poly(Collider& a, Collider& b, Vec2* pushout);

		void update_world_bounds();

		Rect m_world_bounds;
		Vector<Vec2> m_axis;
	};
}
#pragma once

#include <lev/math/vec2.h>
#include <lev/math/polygon.h>
#include <lev/math/rect.h>
#include <lev/math/transform.h>
#include <lev/math/calc.h>
#include <lev/containers/vector.h>
#include <limits>

namespace lev
{
	class Collider2D;

	struct Collision2D
	{
		bool overlaps;
		Collider2D* other;
		Vec2F pushout;
	};

	class Collider2D
	{
	public:
		Collider2D();
		Collider2D(const Collider2D& other);

		Collider2D(const Polygon& poly);
		Collider2D(const RectF& rect);
		Collider2D(float x, float y, float w, float h);
		
		void make_polygon(const Polygon& polygon);
		void make_rect(const RectF& rect);

		Collision2D collide(Collider2D& other);

		Collider2D offset_copy(const Vec2F& offset) const;
		RectF world_bounds();

		Transform* parent;
		Transform transform;

		Polygon polygon;
		Polygon world_polygon;

	private:
		static bool poly_to_poly(Collider2D& a, Collider2D& b, Vec2F* pushout);

		void update();

		RectF m_world_bounds;
		Vector<Vec2F> m_axis;
	};
}

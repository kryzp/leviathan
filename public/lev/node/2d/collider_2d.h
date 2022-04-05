#pragma once

#include <lev/node/2d/node_2d.h>
#include <lev/math/polygon.h>
#include <lev/math/rect.h>
#include <lev/math/transform_2d.h>
#include <lev/containers/vector.h>

namespace lev
{
	class Collider2D;

	struct Collision2D
	{
		bool overlaps;
		Collider2D* other;
		Vec2F pushout;
	};

	class Collider2D : public Node2D
	{
	public:
		Collider2D();
		Collider2D(const Polygon& poly);
		Collider2D(const RectF& rect);
		
		void make_polygon(const Polygon& polygon);
		void make_rect(const RectF& rect);

		Collision2D collide(Collider2D& other);

		Collider2D offset_copy(const Vec2F& offset) const;
		RectF world_bounds();

		Polygon polygon;
		Polygon world_polygon;

	private:
		static bool poly_to_poly(Collider2D& a, Collider2D& b, Vec2F* pushout);

		void update_world_polygon();

		RectF m_world_bounds;
		Vector<Vec2F> m_axis;
	};
}

#pragma once

#include <lev/math/vec2.h>
#include <lev/math/polygon.h>
#include <lev/math/rect.h>
#include <lev/math/transform.h>
#include <lev/math/calc.h>
#include <lev/containers/vector.h>
#include <limits>

// basic 2d collider
// all it does is detect/resolve intersections with other colliders.
// theres no opposite force applied to the other body or anything.
// just a pushout vector is returned with tells one collider how to react to stop colliding.

namespace lev::phys
{
	class Collider2D;

	struct Hit
	{
		Collider2D* other;
		Vec2 pushout;
		bool solid;
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

		Collider2D offset(const Vec2& offset) const;

		RectF get_world_bounds();

		bool overlaps(Collider2D& other, Vec2* pushout = nullptr);

		Transform* parent;
		Transform transform;

		Polygon polygon;
		Polygon world_polygon;

	private:
		static bool poly_to_poly(Collider2D& a, Collider2D& b, Vec2* pushout);

		void update_world_bounds();

		RectF m_world_bounds;
		Vector<Vec2> m_axis;
	};

	Collider2D::Collider2D()
		: parent(nullptr)
		, transform()
		, polygon()
		, world_polygon()
		, m_world_bounds(RectF::zero())
		, m_axis()
	{
	}

	Collider2D::Collider2D(const Collider2D& other)
	{
		this->polygon        = other.polygon;
		this->world_polygon  = other.world_polygon;
		this->m_world_bounds = other.m_world_bounds;
		this->m_axis         = other.m_axis;
		this->transform      = other.transform;
		this->parent         = other.parent;
	}

	Collider2D::Collider2D(const Polygon& polygon)
		: Collider2D()
	{
		make_polygon(polygon);
	}

	Collider2D::Collider2D(const RectF& rect)
		: Collider2D()
	{
		make_rect(rect);
	}

	Collider2D::Collider2D(float x, float y, float w, float h)
		: Collider2D()
	{
		make_rect(RectF(x, y, w, h));
	}

	void Collider2D::make_polygon(const Polygon& poly)
	{
		this->polygon.vertices = poly.vertices;
		this->world_polygon.vertices = poly.vertices;
		this->m_axis = Vector<Vec2>(poly.vertices.size());
	}

	void Collider2D::make_rect(const RectF& rect)
	{
		Polygon rectpoly = Polygon({
			Vec2(0, 0),
			Vec2(rect.w, 0),
			Vec2(rect.w, rect.h),
			Vec2(0, rect.h)
		});

		make_polygon(rectpoly);
		transform.move(Vec2(rect.x, rect.y));
	}

	Collider2D Collider2D::offset(const Vec2& amount) const
	{
		Collider2D collider = *this;
		collider.transform.move(amount);
		return collider;
	}

	RectF Collider2D::get_world_bounds()
	{
		update_world_bounds();
		return m_world_bounds;
	}

	bool Collider2D::overlaps(Collider2D& other, Vec2* pushout)
	{
		Vec2 push;
		bool result = Collider2D::poly_to_poly(*this, other, &push);

		if (pushout)
			(*pushout) = push;

		return result;
	}

	void Collider2D::update_world_bounds()
	{
		int vert_count = polygon.vertices.size();

		Mat3x2 mat = transform.matrix();
		if (parent)
			mat = parent->matrix() * mat;

		// update axis and points
		{
			Vec2 first_vert = world_polygon.vertices[0];

			for (int i = 0; i < vert_count; i++)
			{
				Vec2 curr_vert = world_polygon.vertices[i];
				Vec2 next_vert = first_vert;

				if ((i+1) < vert_count)
					next_vert = world_polygon.vertices[i+1];

				world_polygon.vertices[i] = Vec2::transform(polygon.vertices[i], mat);

				m_axis[i] = (next_vert - curr_vert).normalized().perpendicular();
			}
		}

		// update world bounds
		{
			float minx = std::numeric_limits<float>::max();
			float maxx = std::numeric_limits<float>::lowest();
			float miny = std::numeric_limits<float>::max();
			float maxy = std::numeric_limits<float>::lowest();

			for (int i = 0; i < vert_count; i++)
			{
				float x = world_polygon.vertices[i].x;
				float y = world_polygon.vertices[i].y;
				minx = Calc::min(minx, x);
				maxx = Calc::max(maxx, x);
				miny = Calc::min(miny, y);
				maxy = Calc::max(maxy, y);
			}

			m_world_bounds.x = minx;
			m_world_bounds.y = miny;
			m_world_bounds.w = maxx - minx;
			m_world_bounds.h = maxy - miny;
		}
	}

	bool Collider2D::poly_to_poly(Collider2D& a, Collider2D& b, Vec2* pushout)
	{
		a.update_world_bounds();
		b.update_world_bounds();

		if (!a.m_world_bounds.intersects(b.m_world_bounds))
			return false;

		float length = std::numeric_limits<float>::max();

		for (int i = 0; i < a.m_axis.size(); i++)
		{
			auto axis = a.m_axis[i];
			float amount = 0.0f;

			if (!Polygon::axis_overlaps(a.world_polygon, b.world_polygon, axis, &amount))
				return false;
			
			if (!pushout)
				continue;

			if (Calc::abs(amount) < length)
			{
				(*pushout) = axis * amount;
				length = Calc::abs(amount);
			}
		}

		for (int i = 0; i < b.m_axis.size(); i++)
		{
			auto axis = b.m_axis[i];
			float amount = 0.0f;

			if (!Polygon::axis_overlaps(a.world_polygon, b.world_polygon, axis, &amount))
				return false;
			
			if (!pushout)
				continue;

			if (Calc::abs(amount) < length)
			{
				(*pushout) = axis * amount;
				length = Calc::abs(amount);
			}
		}

		return true;
	}
}

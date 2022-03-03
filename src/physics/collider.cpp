#include <lev/physics/collider.h>
#include <lev/math/calc.h>

#include <limits>

using namespace Lev;

Collider::Collider()
	: polygon()
	, world_polygon()
	, m_world_bounds(RectF::ZERO)
	, m_axis()
{
}

Collider::Collider(const Collider& other)
{
	polygon = other.polygon;
	world_polygon = other.world_polygon;
	m_world_bounds = other.m_world_bounds;
	m_axis = other.m_axis; // note: havent checked but this might be copying the reference so this could corrupt if 'other' goes out of scope
	transform = other.transform;
}

Collider::Collider(const Polygon& polygon)
	: m_axis(polygon.vertices.size())
{
	make_polygon(polygon);
}

Collider::Collider(const RectF& rect)
{
	make_rect(rect);
}

Collider::Collider(float x, float y, float w, float h)
{
	make_rect(RectF(x, y, w, h));
}

void Collider::make_polygon(const Polygon& poly)
{
	this->polygon = Polygon(poly.vertices);
	this->world_polygon = Polygon(poly.vertices);

	for (int i = 0; i < poly.vertices.size(); i++)
		this->m_axis.push_back(Vec2::ZERO);
}

void Collider::make_rect(const RectF& rect)
{
	Polygon rectpoly = Polygon({
		Vec2(0, 0),
		Vec2(rect.w, 0),
		Vec2(rect.w, rect.h),
		Vec2(0, rect.h)
	});

	make_polygon(rectpoly);

	// offset by rectangle position
	transform.position(rect.x, rect.y);
}

Collider Collider::get_offset(const Vec2& offset) const
{
	Collider collider = Collider(*this);
	collider.transform.move(offset);
	return collider;
}

RectF Collider::get_world_bounds()
{
	update_world_bounds();
	return m_world_bounds;
}

bool Collider::overlaps(Collider& other, Vec2* pushout)
{
	Vec2 push;
	bool result = Collider::poly_to_poly(*this, other, &push);

	if (pushout)
		(*pushout) = push;

	return result;
}

void Collider::update_world_bounds()
{
	int vert_count = polygon.vertices.size();
	Mat3x2 mat = transform.matrix();

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

			m_axis[i] = (next_vert - curr_vert).normalized();
			m_axis[i] = m_axis[i].perpendicular();
		}
	}

	// update world bounds
	{
		float minx = std::numeric_limits<float>::max();
		float maxx = std::numeric_limits<float>::lowest(); // i present: asshole design. for some ungodly reason ::min() doesn't return the lowest value, it returns the smallest value. FOR 1 HOUR I HAVE SAT HERE FIXING THIS STUPID BUG BECAUSE THE C++ LIBRARY IS INCAPABLE OF NAMING THINGS. WHY ISNT THIS min() AND THEN A SEPERATE FUNCTION smallest() !!!!! WHY C++ LIBRARY??? WHY WOULD YOU DO THAT????
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

bool Collider::poly_to_poly(Collider& a, Collider& b, Vec2* pushout)
{
	a.update_world_bounds();
	b.update_world_bounds();

	if (!a.m_world_bounds.intersects(b.m_world_bounds))
		return false;

	float distance = 0;

	for (int i = 0; i < a.m_axis.size(); i++)
	{
		Vec2 axis = a.m_axis[i];
		float amount = 0.0f;

		if (!Polygon::axis_overlaps(a.world_polygon, b.world_polygon, axis, &amount))
			return false;

		if (i == 0 || Calc::abs(amount) < distance)
		{
			if (pushout)
				(*pushout) = axis * amount;

			distance = Calc::abs(amount);
		}
	}

	for (int i = 0; i < b.m_axis.size(); i++)
	{
		Vec2 axis = b.m_axis[i];
		float amount = 0.0f;

		if (!Polygon::axis_overlaps(a.world_polygon, b.world_polygon, axis, &amount))
			return false;

		if (i == 0 || Calc::abs(amount) < distance)
		{
			if (pushout)
				(*pushout) = axis * amount;

			distance = Calc::abs(amount);
		}
	}

	return true;
}

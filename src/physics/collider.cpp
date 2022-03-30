#include <lev/physics/collider.h>

using namespace lev;

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
	this->m_axis = Vector<Vec2F>(poly.vertices.size());
}

void Collider2D::make_rect(const RectF& rect)
{
	make_polygon(Polygon({
		Vec2F(rect.x, rect.y),
		Vec2F(rect.w, rect.y),
		Vec2F(rect.w, rect.h),
		Vec2F(rect.x, rect.h)
	}));
}

Collision2D Collider2D::collide(Collider2D& other)
{
	Vec2F push;
	bool overlaps = Collider2D::poly_to_poly(*this, other, &push);
	
	Collision2D data;
	data.overlaps = overlaps;
	data.pushout = push;
	data.other = &other;

	return data;
}

Collider2D Collider2D::offset_copy(const Vec2F& amount) const
{
	Collider2D collider = *this;
	collider.transform.move(amount);
	return collider;
}

RectF Collider2D::world_bounds()
{
	update();
	return m_world_bounds;
}

void Collider2D::update()
{
	int vert_count = polygon.vertices.size();

	Mat3x2 mat = transform.matrix();
	if (parent)
		mat = parent->matrix() * mat;

	// update world polygon
	{
		Vec2F first_vert = world_polygon.vertices[0];

		for (int i = 0; i < vert_count; i++)
		{
			Vec2F curr_vert = world_polygon.vertices[i];
			Vec2F next_vert = first_vert;

			if ((i+1) < vert_count)
				next_vert = world_polygon.vertices[i+1];

			world_polygon.vertices[i] = Vec2F::transform(polygon.vertices[i], mat);

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

bool Collider2D::poly_to_poly(Collider2D& a, Collider2D& b, Vec2F* pushout)
{
	a.update();
	b.update();

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

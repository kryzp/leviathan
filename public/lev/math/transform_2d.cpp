#include <lev/math/transform_2d.h>
#include <lev/math/calc.h>

using namespace lev;

Transform2D::Transform2D()
	: m_matrix(Mat3x2::identity())
	, m_origin(Vec2F::zero())
	, m_position(Vec2F::zero())
	, m_scale(Vec2F::one())
	, m_rotation(0)
	, m_dirty(false)
{
}

Transform2D::Transform2D(const Transform2D& other)
{
	m_matrix = other.m_matrix;
	m_origin = other.m_origin;
	m_position = other.m_position;
	m_scale = other.m_scale;
	m_rotation = other.m_rotation;
	m_dirty = other.m_dirty;
}

Mat3x2 Transform2D::matrix()
{
	if (m_dirty)
	{
		m_matrix = Mat3x2::create_transform(m_position, m_rotation, m_scale, m_origin);;
		m_dirty = false;
	}

	return m_matrix;
}

Transform2D Transform2D::offset(const Vec2F& amount)
{
	Transform2D copy = *this;
	copy.m_position += amount;
	return copy;
}

Vec2F Transform2D::origin() const
{
	return m_origin;
}

Transform2D& Transform2D::origin(const Vec2F& v)
{
	if (v != m_origin)
	{
		m_origin = v;
		m_dirty = true;

		if (on_transformed)
			on_transformed();
	}

	return *this;
}

Transform2D& Transform2D::origin(float x, float y)
{
	return origin(Vec2F(x, y));
}

Vec2F Transform2D::position() const
{
	return m_position;
}

Transform2D& Transform2D::position(const Vec2F& v)
{
	if (v != m_position)
	{
		m_position = v;
		m_dirty = true;

		if (on_transformed)
			on_transformed();
	}

	return *this;
}

Transform2D& Transform2D::position(float x, float y)
{
	return position(Vec2F(x, y));
}

Transform2D& Transform2D::move(const Vec2F& v)
{
	return position(m_position + v);
}

Transform2D& Transform2D::move(float x, float y)
{
	return move(Vec2F(x, y));
}

Vec2F Transform2D::scale() const
{
	return m_scale;
}

Transform2D& Transform2D::scale(const Vec2F& v)
{
	if (v != m_scale)
	{
		m_scale = v;
		m_dirty = true;

		if (on_transformed)
			on_transformed();
	}

	return *this;
}

Transform2D& Transform2D::scale(float x, float y)
{
	return scale(Vec2F(x, y));
}

float Transform2D::rotation() const
{
	return m_rotation;
}

Transform2D& Transform2D::rotation(float r)
{
	if (r != m_rotation)
	{
		m_rotation = r;
		m_dirty = true;

		if (on_transformed)
			on_transformed();
	}

	return *this;
}

Transform2D& Transform2D::rotate(float r)
{
	return rotation(m_rotation + r);
}

float Transform2D::rotation_rad() const
{
	return m_rotation;
}

float Transform2D::rotation_deg() const
{
	return m_rotation * calc::RAD2DEG;
}

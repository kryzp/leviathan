#include <lev/math/transform.h>
#include <lev/math/calc.h>

using namespace lev;

Transform::Transform()
	: m_matrix(Mat3x2::identity())
	, m_origin(Vec2::zero())
	, m_position(Vec2::zero())
	, m_scale(Vec2::one())
	, m_rotation(0)
	, m_dirty(false)
{
}

Transform::Transform(const Transform& other)
{
	m_matrix = other.m_matrix;
	m_origin = other.m_origin;
	m_position = other.m_position;
	m_scale = other.m_scale;
	m_rotation = other.m_rotation;
	m_dirty = other.m_dirty;
}

Mat3x2 Transform::matrix()
{
	if (m_dirty)
	{
		m_matrix = Mat3x2::create_transform(m_position, m_rotation, m_scale, m_origin);;
		m_dirty = false;
	}

	return m_matrix;
}

Transform Transform::offset(const Vec2& amount)
{
	Transform copy = *this;
	copy.m_position += amount;
	return copy;
}

Vec2 Transform::origin() const
{
	return m_origin;
}

Transform& Transform::origin(const Vec2& v)
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

Transform& Transform::origin(float x, float y)
{
	return origin(Vec2(x, y));
}

Vec2 Transform::position() const
{
	return m_position;
}

Transform& Transform::position(const Vec2& v)
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

Transform& Transform::position(float x, float y)
{
	return position(Vec2(x, y));
}

Transform& Transform::move(const Vec2& v)
{
	return position(m_position + v);
}

Transform& Transform::move(float x, float y)
{
	return move(Vec2(x, y));
}

Vec2 Transform::scale() const
{
	return m_scale;
}

Transform& Transform::scale(const Vec2& v)
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

Transform& Transform::scale(float x, float y)
{
	return scale(Vec2(x, y));
}

float Transform::rotation() const
{
	return m_rotation;
}

Transform& Transform::rotation(float r)
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

Transform& Transform::rotate(float r)
{
	return rotation(m_rotation + r);
}

float Transform::rotation_rad() const
{
	return m_rotation;
}

float Transform::rotation_deg() const
{
	return m_rotation * Calc::RAD2DEG;
}

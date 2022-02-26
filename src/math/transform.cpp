#include <lev/math/transform.h>
#include <lev/math/calc.h>

using namespace Lev;

Transform::Transform()
	: m_matrix(Mat3x2::IDENTITY)
	, m_origin(Vec2::ZERO)
	, m_position(Vec2::ZERO)
	, m_scale(Vec2::ONE)
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
		m_matrix = Mat3x2::create_transformation(m_position, m_rotation, m_scale, m_origin);;
		m_dirty = false;
	}

	return m_matrix;
}

Vec2 Transform::origin() const
{
	return m_origin;
}

void Transform::origin(const Vec2& v)
{
	if (v == Vec2::ZERO)
		return;

	m_origin = v;
	m_dirty = true;
}

void Transform::origin(float x, float y)
{
	if (x == 0 && y == 0)
		return;

	m_origin = Vec2(x, y);
	m_dirty = true;
}

void Transform::offset(const Vec2& v)
{
	if (v == Vec2::ZERO)
		return;

	m_origin += v;
	m_dirty = true;
}

Vec2 Transform::position_with_origin() const
{
	return m_position - m_origin;
}

Vec2 Transform::position() const
{
	return m_position;
}

void Transform::position(const Vec2& v)
{
	if (v == Vec2::ZERO)
		return;

	m_position = v;
	m_dirty = true;
}

void Transform::position(float x, float y)
{
	if (x == 0 && y == 0)
		return;

	m_position = Vec2(x, y);
	m_dirty = true;
}

void Transform::move(const Vec2& v)
{
	if (v == Vec2::ZERO)
		return;

	m_position += v;
	m_dirty = true;
}

void Transform::movex(float x)
{
	if (x == 0)
		return;

	m_position.x += x;
	m_dirty = true;
}

void Transform::movey(float y)
{
	if (y == 0)
		return;

	m_position.y += y;
	m_dirty = true;
}

Vec2 Transform::scale() const
{
	return m_scale;
}

void Transform::scale(const Vec2& v)
{
	if (v == Vec2::ZERO)
		return;

	m_scale = v;
	m_dirty = true;
}

void Transform::scale(float x, float y)
{
	if (x == 0 && y == 0)
		return;

	m_scale = Vec2(x, y);
	m_dirty = true;
}

float Transform::rotation() const
{
	return m_rotation;
}

void Transform::rotation(float r)
{
	if (r == 0)
		return;

	m_rotation = r;
	m_dirty = true;
}

void Transform::rotate(float r)
{
	if (r == 0)
		return;

	m_rotation += r;
	m_dirty = true;
}

float Transform::rotation_rad() const
{
	return m_rotation;
}

float Transform::rotation_deg() const
{
	return m_rotation * Calc::RAD2DEG;
}

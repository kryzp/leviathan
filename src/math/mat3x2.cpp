#include <lev/math/mat3x2.h>
#include <lev/math/mat4x4.h>
#include <lev/math/vec2.h>
#include <lev/math/calc.h>

// nice spelling of 'transformation' alan :/
// https://www.alanzucconi.com/2016/02/10/tranfsormation-matrix/

/* Heres how it works:
This is actually just a 2x2 transformation matrix + position (m31, m32)

So, multiplication means doing 2x2 matrix multiplication then also applying
that 'other' matrix to the position vector!
*/

using namespace lev;

Mat3x2::Mat3x2()
	: m11(1), m12(0)
	, m21(0), m22(1)
	, m31(0), m32(0)
{
}

Mat3x2::Mat3x2(const Mat3x2& other)
	: m11(other.m11), m12(other.m12)
	, m21(other.m21), m22(other.m22)
	, m31(other.m31), m32(other.m32)
{
}

Mat3x2::Mat3x2(float initial)
	: m11(initial), m12(0)
	, m21(0), m22(initial)
	, m31(0), m32(0)
{
}

Mat3x2::Mat3x2(
	float m11, float m12,
	float m21, float m22,
	float m31, float m32
)
	: m11(m11), m12(m12)
	, m21(m21), m22(m22)
	, m31(m31), m32(m32)
{
}

float* Mat3x2::value_ptr()
{
	return elements;//&m11;
}

const float* Mat3x2::value_ptr() const
{
	return elements;//&m11;
}

float Mat3x2::determinant() const
{
	return m11*m22 - m21*m12;
}

Mat3x2 Mat3x2::inverse() const
{
	auto inv_det = 1.0f / determinant();

	return Mat3x2(
		 m22 * inv_det,
		-m12 * inv_det,
		-m21 * inv_det,
		 m11 * inv_det,
		(m21*m32 - m31*m22) * inv_det,
		(m31*m12 - m11*m32) * inv_det
	);
}

Mat3x2 Mat3x2::create_skew(const Vec2& amount)
{
	return Mat3x2(
		1, amount.y,
		amount.x, 1,
		0, 0
	);
}

Mat3x2 Mat3x2::create_scale(const Vec2& scale)
{
	return Mat3x2(
		scale.x, 0,
		0, scale.y,
		0, 0
	);
}

Mat3x2 Mat3x2::create_scale(float scale)
{
	return Mat3x2(
		scale, 0,
		0, scale,
		0, 0
	);
}

Mat3x2 Mat3x2::create_rotation(float rotation)
{
	return Mat3x2(
		Calc::cos(rotation), Calc::sin(rotation),
		-Calc::sin(rotation), Calc::cos(rotation),
		0, 0
	);
}

Mat3x2 Mat3x2::create_translation(const Vec2& translation)
{
	return Mat3x2(
		1, 0,
		0, 1,
		translation.x, translation.y
	);
}

Mat3x2 Mat3x2::create_transform(const Vec2& position, float rotation, const Vec2& scale, const Vec2& origin)
{
	Mat3x2 mat = Mat3x2::IDENTITY;

	if (origin.x != 0 || origin.y != 0)
		mat *= create_translation(-origin);

	if (scale.x != 1 || scale.y != 1)
		mat *= create_scale(scale);

	if (rotation != 0)
		mat *= create_rotation(rotation);

	if (position.x != 0 || position.y != 0)
		mat *= create_translation(position);

	return mat;
}

Mat3x2 Mat3x2::operator - (const Mat3x2& other) const
{
	return Mat3x2(
		m11 - other.m11,
		m12 - other.m12,
		m21 - other.m21,
		m22 - other.m22,
		m31 - other.m31,
		m32 - other.m32
	);
}

Mat3x2 Mat3x2::operator + (const Mat3x2& other) const
{
	return Mat3x2(
		m11 + other.m11,
		m12 + other.m12,
		m21 + other.m21,
		m22 + other.m22,
		m31 + other.m31,
		m32 + other.m32
	);
}

Mat3x2 Mat3x2::operator * (const Mat3x2& other) const
{
	return Mat3x2(
		(this->m11 * other.m11) + (this->m12 * other.m21),
		(this->m11 * other.m12) + (this->m12 * other.m22),
		(this->m21 * other.m11) + (this->m22 * other.m21),
		(this->m21 * other.m12) + (this->m22 * other.m22),
		(this->m31 * other.m11) + (this->m32 * other.m21) + other.m31, 
		(this->m31 * other.m12) + (this->m32 * other.m22) + other.m32
	);
}

Mat3x2& Mat3x2::operator -= (const Mat3x2& other)
{
	*this = (*this) - other;
	return *this;
}

Mat3x2& Mat3x2::operator += (const Mat3x2& other)
{
	*this = (*this) + other;
	return *this;
}

Mat3x2& Mat3x2::operator *= (const Mat3x2& other)
{
	*this = (*this) * other;
	return *this;
}

const Mat3x2 Mat3x2::IDENTITY = Mat3x2(
	1, 0,
	0, 1,
	0, 0
);

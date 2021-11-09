#include <lev/math/mat3x2.h>
#include <lev/math/vec2.h>
#include <lev/math/calc.h>

// nice spelling of 'transformation' alan :/
// https://www.alanzucconi.com/2016/02/10/tranfsormation-matrix/

using namespace Lev;

Mat3x2::Mat3x2()
	: m11(0)
	, m12(0)
	, m21(0)
	, m22(0)
	, m31(0)
	, m32(0)
{
}

Mat3x2::Mat3x2(float m11, float m12, float m21, float m22, float m31, float m32)
	: m11(m11)
	, m12(m12)
	, m21(m21)
	, m22(m22)
	, m31(m31)
	, m32(m32)
{
}

float Mat3x2::scaling_factor() const
{
	return Calc::sqrt(m11 * m11 + m12 * m12);
}

float Mat3x2::determinant() const
{
	return (m11 * m22) - (m21 * m12);
}

Mat3x2 Mat3x2::inverse() const
{
	auto inv_det = 1.0f / determinant();

	return Mat3x2(
		 m22 * inv_det,
		-m12 * inv_det,
		-m21 * inv_det,
		 m11 * inv_det,
		(m21 * m32 - m31 * m22) * inv_det,
		(m31 * m12 - m11 * m32) * inv_det
	);
}

Mat3x2 Lev::Mat3x2::create_skew(const Vec2& amount)
{
	Mat3x2 mat;

	mat.m11 = 1;
	mat.m12 = amount.y;
	mat.m21 = amount.x;
	mat.m22 = 1;
	mat.m31 = 0;
	mat.m32 = 0;

	return mat;
}

Mat3x2 Mat3x2::create_scale(const Vec2& scale)
{
	Mat3x2 mat;

	mat.m11 = scale.x;
	mat.m12 = 0;
	mat.m21 = 0;
	mat.m22 = scale.y;
	mat.m31 = 0;
	mat.m32 = 0;

	return mat;
}

Mat3x2 Mat3x2::create_rotation(float rotation)
{
	Mat3x2 mat;

	mat.m11 = Calc::cos(rotation);
	mat.m12 = -Calc::sin(rotation);
	mat.m21 = Calc::sin(rotation);
	mat.m22 = Calc::cos(rotation);
	mat.m31 = 0;
	mat.m32 = 0;

	return mat;
}

Mat3x2 Mat3x2::create_translation(const Vec2& position)
{
	Mat3x2 mat;

	mat.m11 = 1;
	mat.m12 = 0;
	mat.m21 = 0;
	mat.m22 = 1;
	mat.m31 = position.x;
	mat.m32 = position.y;

	return mat;
}

Mat3x2 Mat3x2::create_transformation(const Vec2& position, float rotation, const Vec2& scale, const Vec2& origin)
{
	Mat3x2 mat = IDENTITY;

	if (origin.x != 0 || origin.y != 0)
		mat *= create_translation(-origin);

	if (scale.x != 0 || scale.y != 0)
		mat *= create_scale(scale);

	if (rotation != 0)
		mat *= create_rotation(rotation);

	if (position.x != 0 || position.y != 0)
		mat *= create_translation(position);

	return mat;
}

Mat3x2 Mat3x2::operator - (const Mat3x2& other)
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

Mat3x2 Mat3x2::operator + (const Mat3x2& other)
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

Mat3x2 Mat3x2::operator * (const Mat3x2& other)
{
	return Mat3x2(
		(m11 * other.m11) + (m12 * other.m21),
		(m11 * other.m12) + (m12 * other.m22),
		(m21 * other.m11) + (m22 * other.m21),
		(m21 * other.m12) + (m22 * other.m22),
		(m31 * other.m11) + (m32 * other.m21) + other.m31,
		(m31 * other.m12) + (m32 * other.m22) + other.m32
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
	1, 0, 0,
	0, 1, 0
);

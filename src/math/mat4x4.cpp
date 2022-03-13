#include <lev/math/mat4x4.h>
#include <lev/math/mat3x2.h>
#include <lev/math/calc.h>
#include <iostream>

using namespace lev;

Mat4x4::Mat4x4()
	: m11(0), m12(0), m13(0), m14(0)
	, m21(0), m22(0), m23(0), m24(0)
	, m31(0), m32(0), m33(0), m34(0)
	, m41(0), m42(0), m43(0), m44(0)
{
}

Mat4x4::Mat4x4(float initial)
	: m11(initial), m12(0), m13(0), m14(0)
	, m21(0), m22(initial), m23(0), m24(0)
	, m31(0), m32(0), m33(initial), m34(0)
	, m41(0), m42(0), m43(0), m44(initial)
{
}

Mat4x4::Mat4x4(
	float m11, float m12, float m13, float m14,
	float m21, float m22, float m23, float m24,
	float m31, float m32, float m33, float m34,
	float m41, float m42, float m43, float m44
)
	: m11(m11), m12(m12), m13(m13), m14(m14)
	, m21(m21), m22(m22), m23(m23), m24(m24)
	, m31(m31), m32(m32), m33(m33), m34(m34)
	, m41(m41), m42(m42), m43(m43), m44(m44)
{
}

Mat4x4 Mat4x4::create_orthographic(float width, float height, float near, float far)
{
	return create_orthographic_ext(0.0f, width, height, 0.0f, near, far);
}

Mat4x4 Mat4x4::create_orthographic_ext(float left, float right, float bottom, float top, float near, float far)
{
	Mat4x4 result = Mat4x4::identity();

	result.m11 = 2.0f / (right - left);
	result.m22 = 2.0f / (top - bottom);
	result.m33 = 1.0f / (near - far);
	result.m41 = (left + right) / (left - right);
	result.m42 = (top + bottom) / (bottom - top);
	result.m43 = near / (near - far);
	result.m44 = 1.0f;

	return result;
}

Mat4x4 Mat4x4::from_mat3x2(const Mat3x2& mat)
{
	return Mat4x4(
		mat.m11, mat.m12, 0.0f, 0.0f,
		mat.m21, mat.m22, 0.0f, 0.0f,
		0.0f,    0.0f,    1.0f, 0.0f,
		mat.m31, mat.m32, 0.0f, 1.0f
	);
}

Mat4x4 Mat4x4::create_translation(float x, float y, float z)
{
	return Mat4x4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		x, y, z, 1
	);
}

Mat4x4 Mat4x4::create_scale(float x, float y, float z)
{
	return Mat4x4(
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1
	);
}

float* Mat4x4::value_ptr()
{
	return elements;//&m11;
}

const float* Mat4x4::value_ptr() const
{
	return elements;//&m11;
}

Mat4x4 Mat4x4::operator - (const Mat4x4& other) const
{
	return Mat4x4(
		m11 - other.m11,
		m12 - other.m12,
		m13 - other.m13,
		m14 - other.m14,

		m21 - other.m21,
		m22 - other.m22,
		m23 - other.m23,
		m24 - other.m24,

		m31 - other.m31,
		m32 - other.m32,
		m33 - other.m33,
		m34 - other.m34,

		m41 - other.m41,
		m42 - other.m42,
		m43 - other.m43,
		m44 - other.m44
	);
}

Mat4x4 Mat4x4::operator + (const Mat4x4& other) const
{
	return Mat4x4(
		m11 + other.m11,
		m12 + other.m12,
		m13 + other.m13,
		m14 + other.m14,

		m21 + other.m21,
		m22 + other.m22,
		m23 + other.m23,
		m24 + other.m24,

		m31 + other.m31,
		m32 + other.m32,
		m33 + other.m33,
		m34 + other.m34,

		m41 + other.m41,
		m42 + other.m42,
		m43 + other.m43,
		m44 + other.m44
	);
}

Mat4x4 Mat4x4::operator * (const Mat4x4& other) const
{
	return Mat4x4(
		(this->m11 * other.m11) + (this->m12 * other.m21) + (this->m13 * other.m31) + (this->m14 * other.m41),
		(this->m11 * other.m12) + (this->m12 * other.m22) + (this->m13 * other.m32) + (this->m14 * other.m42),
		(this->m11 * other.m13) + (this->m12 * other.m23) + (this->m13 * other.m33) + (this->m14 * other.m43),
		(this->m11 * other.m14) + (this->m12 * other.m24) + (this->m13 * other.m34) + (this->m14 * other.m44),

		(this->m21 * other.m11) + (this->m22 * other.m21) + (this->m23 * other.m31) + (this->m24 * other.m41),
		(this->m21 * other.m12) + (this->m22 * other.m22) + (this->m23 * other.m32) + (this->m24 * other.m42),
		(this->m21 * other.m13) + (this->m22 * other.m23) + (this->m23 * other.m33) + (this->m24 * other.m43),
		(this->m21 * other.m14) + (this->m22 * other.m24) + (this->m23 * other.m34) + (this->m24 * other.m44),

		(this->m31 * other.m11) + (this->m32 * other.m21) + (this->m33 * other.m31) + (this->m34 * other.m41),
		(this->m31 * other.m12) + (this->m32 * other.m22) + (this->m33 * other.m32) + (this->m34 * other.m42),
		(this->m31 * other.m13) + (this->m32 * other.m23) + (this->m33 * other.m33) + (this->m34 * other.m43),
		(this->m31 * other.m14) + (this->m32 * other.m24) + (this->m33 * other.m34) + (this->m34 * other.m44),

		(this->m41 * other.m11) + (this->m42 * other.m21) + (this->m43 * other.m31) + (this->m44 * other.m41),
		(this->m41 * other.m12) + (this->m42 * other.m22) + (this->m43 * other.m32) + (this->m44 * other.m42),
		(this->m41 * other.m13) + (this->m42 * other.m23) + (this->m43 * other.m33) + (this->m44 * other.m43),
		(this->m41 * other.m14) + (this->m42 * other.m24) + (this->m43 * other.m34) + (this->m44 * other.m44)
	);
}

Mat4x4& Mat4x4::operator -= (const Mat4x4& other)
{
	*this = (*this) - other;
	return *this;
}

Mat4x4& Mat4x4::operator += (const Mat4x4& other)
{
	*this = (*this) + other;
	return *this;
}

Mat4x4& Mat4x4::operator *= (const Mat4x4& other)
{
	*this = (*this) * other;
	return *this;
}

const Mat4x4& Mat4x4::identity()
{
	static const Mat4x4 IDENTITY = Mat4x4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);

	return IDENTITY;
}

#include <lev/math/mat4x4.h>
#include <lev/math/calc.h>
#include <iostream>

using namespace Lev;

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

float* Mat4x4::value_ptr()
{
	return elements; //&m11;
}

const float* Mat4x4::value_ptr() const
{
	return elements; //&m11;
}

Mat4x4 Mat4x4::operator - (const Mat4x4& other)
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

Mat4x4 Mat4x4::operator + (const Mat4x4& other)
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

Mat4x4 Mat4x4::operator * (const Mat4x4& other)
{
	Mat4x4 result;

	int row = 0;

	for (int i = 0; i < 15; i++)
	{
		float sum = 0.0f;

		for (int j = 0; j < 4; j++)
		{
			int elm0 = row*4 + j;
			int elm1 = (i%4) + (j*4);

			sum += this->elements[elm0] * other.elements[elm1];
		}

		if ((i+1) % 4 == 0)
			row++;

		result.elements[i] = sum;
	}

	return result;
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

const Mat4x4 Mat4x4::IDENTITY = Mat4x4(
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
);

#include <lev/math/vec2.h>
#include <lev/math/calc.h>
#include <lev/math/pair.h>

using namespace lev;

Vec2::Vec2()
{
}

Vec2::Vec2(float x)
	: x(x)
	, y(x)
{
}

Vec2::Vec2(float x, float y)
	: x(x)
	, y(y)
{
}

Vec2 Vec2::transform(const Vec2& vec, const Mat3x2& mat)
{
	return Vec2(
		(vec.x * mat.m11) + (vec.y * mat.m21) + mat.m31,
		(vec.x * mat.m12) + (vec.y * mat.m22) + mat.m32
	);
}

float Vec2::dot(const Vec2& a, const Vec2& b)
{
	return (a.x * b.x) + (a.y * b.y);
}

Vec2 Vec2::from_angle(float angle, float length)
{
	return Vec2(
		Calc::cos(angle) * length,
		Calc::sin(angle) * length
	);
}

Vec2 Vec2::lerp(const Vec2& from, const Vec2& to, float amount)
{
	return Vec2(
		Calc::lerp(from.x, to.x, amount),
		Calc::lerp(from.y, to.y, amount)
	);
}

Float2 Vec2::to_float2() const
{
	return Float2(x, y);
}

float Vec2::angle() const
{
	return Calc::atan2(y, x);
}

float Vec2::length() const
{
	return Calc::sqrt(length_squared());
}

float Vec2::length_squared() const
{
	return (x * x) + (y * y);
}

Vec2 Vec2::normalized() const
{
	float len = length();

	return Vec2(
		x / len,
		y / len
	);
}

Vec2 Vec2::perpendicular() const
{
	return Vec2(-y, x);
}

bool Vec2::operator == (const Vec2& other) const { return this->x == other.x && this->y == other.y; }
bool Vec2::operator != (const Vec2& other) const { return !(*this == other); }

Vec2 Vec2::operator + (const Vec2& other) const { return Vec2( this->x + other.x,  this->y + other.y); }
Vec2 Vec2::operator - (const Vec2& other) const { return Vec2( this->x - other.x,  this->y - other.y); }
Vec2 Vec2::operator - ()                  const { return Vec2(-this->x,           -this->y          ); }
Vec2 Vec2::operator * (const Vec2& other) const { return Vec2( this->x * other.x,  this->y * other.y); }
Vec2 Vec2::operator / (const Vec2& other) const { return Vec2( this->x / other.x,  this->y / other.y); }

Vec2& Vec2::operator += (const Vec2& other) { this->x += other.x; this->y += other.y; return *this; }
Vec2& Vec2::operator -= (const Vec2& other) { this->x -= other.x; this->y -= other.y; return *this; }
Vec2& Vec2::operator *= (const Vec2& other) { this->x *= other.x; this->y *= other.y; return *this; }
Vec2& Vec2::operator /= (const Vec2& other) { this->x /= other.x; this->y /= other.y; return *this; }

const Vec2 Vec2::ZERO  = Vec2( 0,  0);
const Vec2 Vec2::ONE   = Vec2( 1,  1);
const Vec2 Vec2::LEFT  = Vec2(-1,  0);
const Vec2 Vec2::RIGHT = Vec2( 1,  0);
const Vec2 Vec2::UP    = Vec2( 0, -1);
const Vec2 Vec2::DOWN  = Vec2( 0,  1);

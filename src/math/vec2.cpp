#include <lev/math/vec2.h>
#include <lev/math/calc.h>

using namespace LEV;

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

float Vec2::dot(const Vec2& a, const Vec2& b)
{
	return (a.x * b.x) + (a.y * b.y);
}

#include <lev/math/line.h>
#include <lev/math/calc.h>

using namespace lv;

Line::Line()
	: a(Vec2F::zero())
	, b(Vec2F::zero())
{
}

Line::Line(const Vec2F& a, const Vec2F& b)
	: a(a)
	, b(b)
{
}

Line::Line(const Vec2F& pos, float angle, float len)
	: a(pos)
	, b(pos + Vec2F(calc::cos(angle) * len, calc::sin(angle) * len))
{
}

Vec2F Line::delta() const
{
	return (b - a);
}

Vec2F Line::direction() const
{
	return (b - a).normalized();
}

float Line::length() const
{
	return (b - a).length();
}

float Line::length_squared() const
{
	return (b - a).length_squared();
}

float Line::angle() const
{
	return (b - a).angle();
}

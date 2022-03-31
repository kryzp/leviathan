#include <lev/math/line.h>
#include <lev/math/calc.h>

using namespace lev;

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
	, b(pos + Vec2F(Calc::cos(angle) * len, Calc::sin(angle) * len))
{
}

float Line::length() const
{
	return (b - a).length();
}

float Line::length_squared() const
{
	return (b - a).length_squared();
}

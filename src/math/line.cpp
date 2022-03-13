#include <lev/math/line.h>
#include <lev/math/calc.h>

using namespace lev;

Line::Line()
	: a(Vec2::zero())
	, b(Vec2::zero())
{
}

Line::Line(const Vec2& a, const Vec2& b)
	: a(a)
	, b(b)
{
}

Line::Line(const Vec2& pos, float angle, float len)
	: a(pos)
	, b(pos + Vec2(Calc::cos(angle) * len, Calc::sin(angle) * len))
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

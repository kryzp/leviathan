#include <lev/math/triangle.h>

using namespace lev;

Triangle::Triangle()
	: a(Vec2::ZERO)
	, b(Vec2::ZERO)
	, c(Vec2::ZERO)
{
}

Triangle::Triangle(const Vec2& a, const Vec2& b, const Vec2& c)
	: a(a)
	, b(b)
	, c(c)
{
}

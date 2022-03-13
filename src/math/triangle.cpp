#include <lev/math/triangle.h>

using namespace lev;

Triangle::Triangle()
	: a(Vec2::zero())
	, b(Vec2::zero())
	, c(Vec2::zero())
{
}

Triangle::Triangle(const Vec2& a, const Vec2& b, const Vec2& c)
	: a(a)
	, b(b)
	, c(c)
{
}

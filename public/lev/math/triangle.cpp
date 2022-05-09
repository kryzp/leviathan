#include <lev/math/triangle.h>

using namespace lv;

Triangle::Triangle()
	: a(Vec2F::zero())
	, b(Vec2F::zero())
	, c(Vec2F::zero())
{
}

Triangle::Triangle(const Vec2F& a, const Vec2F& b, const Vec2F& c)
	: a(a)
	, b(b)
	, c(c)
{
}

#include <lev/math/quad.h>
#include <lev/math/rect.h>

using namespace lev;

Quad::Quad()
{
}

Quad::Quad(const Vec2F& a, const Vec2F& b, const Vec2F& c, const Vec2F& d)
	: a(a)
	, b(b)
	, c(c)
	, d(d)
{
}

/*
[1]----[4]
 |      |
 |      |
[2]----[3]
*/

Quad::Quad(const RectF& rect)
	: a(rect.top_left())
	, b(rect.bottom_left())
	, c(rect.bottom_right())
	, d(rect.top_right())
{
}

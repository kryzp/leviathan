#include <lev/math/quad.h>
#include <lev/math/rect.h>

using namespace Lev;

Quad::Quad()
{
}

Quad::Quad(const Vec2& a, const Vec2& b, const Vec2& c, const Vec2& d)
	: a(a)
	, b(b)
	, c(c)
	, d(d)
{
}

Quad::Quad(const RectF& rect)
	: a(rect.top_left())
	, b(rect.top_right())
	, c(rect.bottom_left())
	, d(rect.bottom_right())
{
}

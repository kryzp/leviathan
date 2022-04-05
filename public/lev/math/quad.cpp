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

Quad::Quad(const RectF& rect)
	: a(rect.bottom_right())
	, b(rect.top_right())
	, c(rect.top_left())
	, d(rect.bottom_left())
{
}

Quad Quad::operator / (const Vec2F& factor) const
{
	return Quad(
		a / factor,
		b / factor,
		c / factor,
		d / factor
	);
}

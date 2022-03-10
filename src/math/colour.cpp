#include <lev/math/colour.h>

using namespace lev;

Colour::Colour()
	: r(0)
	, g(0)
	, b(0)
	, a(0)
{
}

Colour::Colour(float r, float g, float b, float a)
	: r(r)
	, g(g)
	, b(b)
	, a(a)
{
}

void Colour::premultiply()
{
	r *= a;
	g *= a;
	b *= a;
}

Colour Colour::premultiplied() const
{
	return Colour(
		r * a,
		g * a,
		b * a,
		a
	);
}

bool Colour::operator == (const Colour& other) const
{
	return (
		r == other.r &&
		g == other.g &&
		b == other.b &&
		a == other.a
	);
}

bool Colour::operator != (const Colour& other) const
{
	return !(*this == other);
}

Colour Colour::operator - () const
{
	return Colour(
		1.0f - r,
		1.0f - g,
		1.0f - b,
		a
	);
}

Colour Colour::operator * (float mult) const
{
	return Colour(
		(u8)((float)r * mult),
		(u8)((float)g * mult),
		(u8)((float)b * mult),
		(u8)((float)a * mult)
	);
}

Colour& Colour::operator *= (float mult)
{
	*this = *this * mult;
	return *this;
}

const Colour Colour::EMPTY   = Colour(0, 0, 0, 0);
const Colour Colour::WHITE   = Colour(1, 1, 1, 1);
const Colour Colour::BLACK   = Colour(0, 0, 0, 1);
const Colour Colour::RED     = Colour(1, 0, 0, 1);
const Colour Colour::GREEN   = Colour(0, 1, 0, 1);
const Colour Colour::BLUE    = Colour(0, 0, 1, 1);
const Colour Colour::YELLOW  = Colour(1, 1, 0, 1);
const Colour Colour::MAGENTA = Colour(1, 0, 1, 1);
const Colour Colour::CYAN    = Colour(0, 1, 1, 1);

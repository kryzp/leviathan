#include <lev/math/colour.h>

using namespace Lev;

Colour::Colour()
{
}

Colour::Colour(u8 r, u8 g, u8 b, u8 a)
{
}

void Colour::premultiply()
{
	r = r * a / 255;
	g = g * a / 255;
	b = b * a / 255;
}

Colour Colour::premultiplied() const
{
	return Colour(
		r * a / 255,
		g * a / 255,
		b * a / 255,
		a
	);
}

bool Colour::operator == (const Colour& other) const
{
	return (
		r == other.r,
		g == other.g,
		b == other.b,
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
		255 - r,
		255 - g,
		255 - b,
		a
	);
}

Colour Colour::operator * (float mult) const
{
	return Colour(
		r * mult,
		g * mult,
		b * mult,
		a * mult
	);
}

Colour& Colour::operator *= (float mult)
{
	*this = *this * mult;
	return *this;
}

const Colour Colour::EMPTY   = Colour(0,   0,   0,   0  );
const Colour Colour::WHITE   = Colour(255, 255, 255, 255);
const Colour Colour::BLACK   = Colour(0,   0,   0,   255);
const Colour Colour::RED     = Colour(255, 0,   0,   255);
const Colour Colour::GREEN   = Colour(0,   255, 0,   255);
const Colour Colour::BLUE    = Colour(0,   0,   255, 255);
const Colour Colour::YELLOW  = Colour(255, 255, 0,   255);
const Colour Colour::MAGENTA = Colour(255, 0,   255, 255);
const Colour Colour::CYAN    = Colour(0,   255, 255, 255);

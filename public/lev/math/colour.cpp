#include <lev/math/colour.h>
#include <lev/math/calc.h>

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

Colour::Colour(u32 packed)
	: r(static_cast<u8>(packed >> 24) / 255.0f)
	, g(static_cast<u8>(packed >> 16) / 255.0f)
	, b(static_cast<u8>(packed >> 8) / 255.0f)
	, a(static_cast<u8>(packed >> 0) / 255.0f)
{
}

u32 Colour::pack(const Colour& colour)
{
	u8 r = static_cast<u8>(colour.r * 255.0f);
	u8 g = static_cast<u8>(colour.g * 255.0f);
	u8 b = static_cast<u8>(colour.b * 255.0f);
	u8 a = static_cast<u8>(colour.a * 255.0f);

	return
		r << 24 |
		g << 16 |
		b << 8  |
		a << 0;
}

Colour Colour::from_hsv(float hue, float sat, float val, float alpha)
{
	float C = sat * val;
	float X = C * (1 - calc::abs(calc::mod(hue / 60.0f, 2) - 1));

	float r, g, b;
	r = g = b = 0.0f;

	if (0 <= hue && hue < 60)
	{
		r = C;
		g = X;
		b = 0.0f;
	}
	else if (60 <= hue && hue < 120)
	{
		r = X;
		g = C;
		b = 0.0f;
	}
	else if (120 <= hue && hue < 180)
	{
		r = 0.0f;
		g = C;
		b = X;
	}
	else if (180 <= hue && hue < 240)
	{
		r = 0.0f;
		g = X;
		b = C;
	}
	else if (240 <= hue && hue < 300)
	{
		r = X;
		g = 0.0f;
		b = C;
	}
	else if (300 <= hue && hue < 360)
	{
		r = C;
		g = 0.0f;
		b = X;
	}

	return Colour(r, g, b, alpha);
}

Colour Colour::from_u8(u8 r, u8 g, u8 b, u8 a)
{
	return Colour(
		static_cast<float>(r) / 255.0f,
		static_cast<float>(g) / 255.0f,
		static_cast<float>(b) / 255.0f,
		static_cast<float>(a) / 255.0f
	);
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

Colour Colour::operator * (float factor) const
{
	return Colour(
		r * factor,
		g * factor,
		b * factor,
		a * factor
	);
}

Colour& Colour::operator *= (float factor)
{
	(*this) = (*this) * factor;
	return *this;
}

const Colour& Colour::empty()   { static const Colour EMPTY   = Colour(0, 0, 0, 0); return EMPTY;   }
const Colour& Colour::black()   { static const Colour BLACK   = Colour(0, 0, 0, 1); return BLACK;   }
const Colour& Colour::white()   { static const Colour WHITE   = Colour(1, 1, 1, 1); return WHITE;   }
const Colour& Colour::red()     { static const Colour RED     = Colour(1, 0, 0, 1); return RED;     }
const Colour& Colour::green()   { static const Colour GREEN   = Colour(0, 1, 0, 1); return GREEN;   }
const Colour& Colour::blue()    { static const Colour BLUE    = Colour(0, 0, 1, 1); return BLUE;    }
const Colour& Colour::yellow()  { static const Colour YELLOW  = Colour(1, 1, 0, 1); return YELLOW;  }
const Colour& Colour::magenta() { static const Colour MAGENTA = Colour(1, 0, 1, 1); return MAGENTA; }
const Colour& Colour::cyan()    { static const Colour CYAN    = Colour(0, 1, 1, 1); return CYAN;    }

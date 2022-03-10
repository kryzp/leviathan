#pragma once

#include <lev/core/util.h>

namespace lev
{
	struct Colour
	{
		static const Colour EMPTY;
		static const Colour WHITE;
		static const Colour BLACK;
		static const Colour RED;
		static const Colour GREEN;
		static const Colour BLUE;
		static const Colour YELLOW;
		static const Colour MAGENTA;
		static const Colour CYAN;

		float r;
		float g;
		float b;
		float a;

		Colour();
		Colour(float r, float g, float b, float a = 1.0f);

		void premultiply();
		Colour premultiplied() const;

		bool operator == (const Colour& other) const;
		bool operator != (const Colour& other) const;

		Colour operator - () const;
		Colour operator * (float mult) const;
		Colour& operator *= (float mult);
	};

	// hamburger spelling
	using Color = Colour;
}

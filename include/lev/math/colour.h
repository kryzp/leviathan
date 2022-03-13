#pragma once

#include <lev/core/util.h>

namespace lev
{
	struct Colour
	{
		union
		{
			struct
			{
				float r;
				float g;
				float b;
				float a;
			};

			float elements[4];
		};

		Colour();
		Colour(float r, float g, float b, float a = 1.0f);
		Colour(u32 packed);

		static const Colour& empty();
		static const Colour& black();
		static const Colour& white();
		static const Colour& red();
		static const Colour& green();
		static const Colour& blue();
		static const Colour& yellow();
		static const Colour& magenta();
		static const Colour& cyan();

		// packs all floats into a single 32-bit integer, loses precision
		static u32 pack(const Colour& colour);

		static Colour from_u8(u8 r, u8 g, u8 b, u8 a = 255);

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

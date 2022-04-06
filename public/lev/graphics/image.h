#pragma once

#include <lev/core/util.h>
#include <lev/math/colour.h>
#include <lev/math/rect.h>

#include <functional>

namespace lev
{
	class Image
	{
	public:
		using PaintFn = std::function<Colour(u32, u32)>;

		Image();
		Image(const char* path);
		Image(int width, int height);
		~Image();

		void load(const char* path);
		void free();

		void paint(PaintFn fn);
		void paint(const RectI& rect, PaintFn fn);

		void pixels(const Colour* data);
		void pixels(const Colour* data, u64 pixel_count);
		void pixels(const Colour* data, u64 offset, u64 pixel_count);
		Colour* pixels();
		const Colour* pixels() const;

		int width() const;
		int height() const;
		int nr_channels() const;
		
	private:
		Colour* m_pixels;

		int m_width;
		int m_height;
		int m_nr_channels;

		bool m_stbi_management;
	};
};

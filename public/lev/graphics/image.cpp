#include <lev/graphics/image.h>
#include <lev/io/file_stream.h>

#define STB_IMAGE_IMPLEMENTATION
#include <third_party/stb/stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <third_party/stb/stb_image_write.h>

using namespace lev;

static void lev_stbi_write(void* context, void* data, int size)
{
	((Stream*)context)->write((char*)data, size);
}

Image::Image()
	: m_pixels(nullptr)
	, m_width(0)
	, m_height(0)
	, m_nr_channels(0)
	, m_stbi_management(false)
{
}

Image::Image(const char* path)
{
	load(path);
}

Image::Image(int width, int height)
	: m_width(width)
	, m_height(height)
	, m_nr_channels(0)
	, m_stbi_management(false)
{
	m_pixels = new Colour[width * height];
}

Image::~Image()
{
	free();
}

void Image::load(const char* path)
{
	m_stbi_management = true;

	int w, h, nrc;
	m_pixels = (Colour*)stbi_load(path, &w, &h, &nrc, 4);
	m_width = w;
	m_height = h;
	m_nr_channels = nrc;
}

void Image::free()
{
	if (!m_pixels)
		return;

	if (m_stbi_management)
		stbi_image_free(m_pixels);
	else
		delete[] m_pixels;

	m_pixels = nullptr;
}

void Image::paint(PaintFn fn)
{
	paint(RectI(0, 0, m_width, m_height), fn);
}

void Image::paint(const RectI& rect, PaintFn fn)
{
	for (u32 y = 0; y < rect.h; y++)
	{
		for (u32 x = 0; x < rect.w; x++)
		{
			int yy = rect.y + y;
			int xx = rect.x + x;

			m_pixels[(yy * m_width) + xx] = fn(x, y);
		}
	}
}

void Image::pixels(const Colour* data)
{
	mem::copy(m_pixels, data, sizeof(Colour) * (m_width * m_height));
}

void Image::pixels(const Colour* data, u64 pixel_count)
{
	mem::copy(m_pixels, data, sizeof(Colour) * pixel_count);
}

void Image::pixels(const Colour* data, u64 offset, u64 pixel_count)
{
	mem::copy(m_pixels, data + offset, sizeof(Colour) * pixel_count);
}

bool Image::save_png(const char* file) const
{
	FileStream fs(file, "wb");
	return save_png(fs);
}

bool Image::save_png(Stream& stream) const
{
	LEV_ASSERT(m_pixels, "Pixel data cannot be null");
	LEV_ASSERT(m_width > 0 && m_height > 0, "Width and Height must be > 0");

	stbi_write_force_png_filter = 0;
	stbi_write_png_compression_level = 0;

	if (stbi_write_png_to_func(lev_stbi_write, &stream, m_width, m_height, 4, m_pixels, m_width * 4) != 0)
		return true;
	else
		LEV_ERROR("stbi_write_png_to_func(...) failed");

	return false;
}

bool Image::save_jpg(const char* file, int quality) const
{
	FileStream fs(file, "wb");
	return save_jpg(fs, quality);
}

bool Image::save_jpg(Stream& stream, int quality) const
{
	LEV_ASSERT(m_pixels, "Pixel data cannot be null");
	LEV_ASSERT(m_width > 0 && m_height > 0, "Width and Height must be > 0");

	if (quality < 1)
	{
		log::warn("JPG quality value should be between [1 -> 100]");
		quality = 1;
	}
	else if (quality > 100)
	{
		log::warn("JPG quality value should be between [1 -> 100]");
		quality = 100;
	}

	if (stbi_write_jpg_to_func(lev_stbi_write, &stream, m_width, m_height, 4, m_pixels, quality) != 0)
		return true;
	else
		LEV_ERROR("stbi_write_jpg_to_func(...) failed");

	return false;
}

Colour Image::pixel_at(u32 x, u32 y) const
{
	return m_pixels[(y * m_width) + x];
}

Colour* Image::pixels()
{
	return m_pixels;
}

const Colour* Image::pixels() const
{
	return m_pixels;
}

byte* Image::raw_pixel_data()
{
    return (byte*)m_pixels;
}

const byte* Image::raw_pixel_data() const
{
    return (const byte*)m_pixels;
}

u32 Image::width() const
{
	return m_width;
}

u32 Image::height() const
{
	return m_height;
}

int Image::nr_channels() const
{
	return m_nr_channels;
}

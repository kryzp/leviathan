#include <lev/graphics/image.h>

#define STB_IMAGE_IMPLEMENTATION
#include <third_party/stb/stb_image.h>

using namespace lev;
using namespace lev::gfx;

Image::Image()
{
}

Image::Image(const char* path)
{
	load(path);
}

Image::~Image()
{
	free();
}

void Image::load(const char* path)
{
	int w, h, nrc;

	m_data = stbi_load(path, &w, &h, &nrc, 0);
	m_width = w;
	m_height = h;
	m_nr_channels = nrc;
}

void Image::free() const
{
	stbi_image_free(m_data);
}

const byte* Image::data() const
{
	return m_data;
}

int Image::width() const
{
	return m_width;
}

int Image::height() const
{
	return m_height;
}

int Image::nr_channels() const
{
	return m_nr_channels;
}

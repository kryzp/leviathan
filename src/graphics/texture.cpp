#include <lev/graphics/texture.h>
#include <backend/graphics.h>

using namespace Lev;

Ref<Texture> Texture::create(const char* path)
{
	return create(Image(path));
}

Ref<Texture> Texture::create(const Image& image)
{
	return create(image.width(), image.height(), image.data(), TextureFormat::RGBA);
}

Ref<Texture> Texture::create(int width, int height, const byte* data, TextureFormat format)
{
	Ref<Texture> result = Graphics::create_texture(width, height, format);
	result->m_width = width;
	result->m_height = height;
	result->set(data);

	return result;
}

int Texture::width() const
{
	return m_width;
}

int Texture::height() const
{
	return m_height;
}

u32 Texture::id() const
{
	return m_id;
}

#include <lev/graphics/texture.h>
#include <backend/renderer.h>

using namespace lev;

const TextureSampler& TextureSampler::pixel()
{
	static const TextureSampler PIXEL(
		TEXTURE_FILTER_NEAREST,
		TEXTURE_WRAP_CLAMP,
		TEXTURE_WRAP_CLAMP
	);

	return PIXEL;
}

const TextureSampler& TextureSampler::linear()
{
	static const TextureSampler LINEAR(
		TEXTURE_FILTER_LINEAR,
		TEXTURE_WRAP_CLAMP,
		TEXTURE_WRAP_CLAMP
	);

	return LINEAR;
}

////////////////////////////////////////////////////////////////////////////

Texture::Texture()
{
}

Texture::~Texture()
{
}

Ref<Texture> Texture::create(const char* path)
{
	return create(Image(path));
}

Ref<Texture> Texture::create(const Image& image)
{
	return create(image.width(), image.height(), TEXTURE_FORMAT_RGBA, TEXTURE_TYPE_UNSIGNED_BYTE, (const byte*)image.pixels());
}

Ref<Texture> Texture::create(int width, int height, TextureFormat format, TextureType type, const byte* data)
{
	LEV_ASSERT(width > 0 && height > 0, "Width and Height must be greater than 0");

	Ref<Texture> result = Renderer::create_texture(TextureData(width, height, format, type));

	if (data)
		result->generate(data);

	return result;
}

void Texture::unbind()
{
	Renderer::unbind_texture();
}

void Texture::unbind_image()
{
	Renderer::unbind_texture_image();
}

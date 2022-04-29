#include <lev/graphics/texture.h>
#include <backend/renderer.h>

using namespace lev;

const TextureSampler& TextureSampler::pixel()
{
	static const TextureSampler PIXEL(
		TEX_FILTER_NEAREST,
		TEX_WRAP_CLAMP,
		TEX_WRAP_CLAMP
	);

	return PIXEL;
}

const TextureSampler& TextureSampler::linear()
{
	static const TextureSampler LINEAR(
		TEX_FILTER_LINEAR,
		TEX_WRAP_CLAMP,
		TEX_WRAP_CLAMP
	);

	return LINEAR;
}

////////////////////////////////////////////////////////////////////////////

Texture* Texture::create(const char* path)
{
	LEV_ASSERT(path, "Path must not be nullptr");
	return create(Image(path), TEX_FMT_RGBA, I_TEX_FMT_RGBA32F, TEX_TYPE_UNSIGNED_BYTE);
}

Texture* Texture::create(const Image& image, u8 format, u8 internal_format, u8 type)
{
	return create(image.width(), image.height(), format, internal_format, type, image.raw_pixel_data());
}

Texture* Texture::create(u32 width, u32 height, u8 format, u8 internal_format, u8 type, const byte* data)
{
	Texture* result = Renderer::inst()->create_texture(
		TextureData(width, height, format, internal_format, type)
	);

	if (data)
		result->generate(data);

	return result;
}

void Texture::unbind()
{
	Renderer::inst()->unbind_texture();
}

void Texture::unbind_image()
{
	Renderer::inst()->unbind_texture_image();
}

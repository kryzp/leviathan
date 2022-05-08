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

Ref<Texture> Texture::create(const char* path)
{
	LEV_ASSERT(path, "Path must not be nullptr");
	return create(Image(path), TEX_FMT_RGBA, TEX_I_FMT_RGBA32F, TEX_TYPE_UNSIGNED_BYTE);
}

Ref<Texture> Texture::create(const Image& image, u8 format, u8 internal_format, u8 type)
{
	return create(image.width(), image.height(), format, internal_format, type, image.raw_pixel_data());
}

Ref<Texture> Texture::create(u32 width, u32 height, u8 format, u8 internal_format, u8 type, const byte* data)
{
	Ref<Texture> result = bknd::Renderer::inst()->create_texture(width, height, TextureFormatInfo(format, internal_format, type));

	if (data)
		result->generate(data);

	return result;
}

void Texture::unbind()
{
	bknd::Renderer::inst()->unbind_texture();
}

void Texture::unbind_image()
{
	bknd::Renderer::inst()->unbind_texture_image();
}

////////////////////////////////////////////////////////////////////////////

Ref<ArrayTexture> ArrayTexture::create(const char* path, u32 image_count)
{
	LEV_ASSERT(path, "Path must not be nullptr");
	return create(Image(path), TEX_FMT_RGBA, TEX_I_FMT_RGBA32F, TEX_TYPE_UNSIGNED_BYTE, image_count);
}

Ref<ArrayTexture> ArrayTexture::create(const Image& image, u8 format, u8 internal_format, u8 type, u32 image_count)
{
	return create(image.width(), image.height(), format, internal_format, type, image_count, image.raw_pixel_data());
}

Ref<ArrayTexture> ArrayTexture::create(u32 width, u32 height, u8 format, u8 internal_format, u8 type, u32 image_count, const byte* data)
{
	Ref<ArrayTexture> result = bknd::Renderer::inst()->create_array_texture(
		width,
		height,
		TextureFormatInfo(format, internal_format, type),
		image_count
	);

	if (data)
		result->generate(data);

	return result;
}

void ArrayTexture::unbind()
{
	bknd::Renderer::inst()->unbind_array_texture();
}

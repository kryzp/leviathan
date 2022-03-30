#include <lev/graphics/texture.h>
#include <backend/renderer.h>

using namespace lev;

const TextureSampler& TextureSampler::pixel()
{
	static const TextureSampler PIXEL(
		TextureFilter::NEAREST,
		TextureWrap::CLAMP,
		TextureWrap::CLAMP
	);

	return PIXEL;
}

const TextureSampler& TextureSampler::linear()
{
	static const TextureSampler LINEAR(
		TextureFilter::LINEAR,
		TextureWrap::CLAMP,
		TextureWrap::CLAMP
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
	return create(image.width(), image.height(), TextureFormat::RGBA, (const byte*)image.pixels());
}

Ref<Texture> Texture::create(int width, int height, TextureFormat format, const byte* data)
{
	LEV_ASSERT(width > 0 && height > 0, "Width and Height must be greater than 0");

	Ref<Texture> result = Renderer::create_texture(TextureData(width, height, format));

	if (data)
		result->generate(data);

	return result;
}

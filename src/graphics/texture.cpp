#include <lev/graphics/texture.h>
#include <backend/renderer.h>

using namespace lev;

const TextureSampler& TextureSampler::pixel()
{
	static const TextureSampler PIXEL = {
		.filter = TextureFilter::NEAREST,
		.wrap_x = TextureWrap::CLAMP,
		.wrap_y = TextureWrap::CLAMP
	};

	return PIXEL;
}

const TextureSampler& TextureSampler::linear()
{
	static const TextureSampler LINEAR = {
		.filter = TextureFilter::LINEAR,
		.wrap_x = TextureWrap::CLAMP,
		.wrap_y = TextureWrap::CLAMP
	};

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
	TextureData tex = {
		.width = width,
		.height = height,
		.format = format
	};

	Ref<Texture> result = Renderer::create_texture(tex);

	if (data)
		result->generate(data);

	return result;
}

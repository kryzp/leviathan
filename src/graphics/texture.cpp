#include <lev/graphics/texture.h>
#include <backend/renderer.h>

using namespace lev;
using namespace lev::gfx;

const TextureSampler& TextureSampler::pixel()
{
	static const TextureSampler PIXEL = {
		.filter = gfx::TextureFilter::NEAREST,
		.wrap_x = gfx::TextureWrap::CLAMP,
		.wrap_y = gfx::TextureWrap::CLAMP
	};

	return PIXEL;
}

const TextureSampler& TextureSampler::linear()
{
	static const TextureSampler LINEAR = {
		.filter = gfx::TextureFilter::LINEAR,
		.wrap_x = gfx::TextureWrap::CLAMP,
		.wrap_y = gfx::TextureWrap::CLAMP
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
	return create(image.width(), image.height(), TextureFormat::RGBA, image.data());
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

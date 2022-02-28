#include <lev/graphics/texture.h>
#include <backend/renderer.h>

using namespace Lev;

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
	return create(image.width(), image.height(), image.data(), TextureFormat::RGBA);
}

Ref<Texture> Texture::create(int width, int height, const byte* data, TextureFormat format)
{
	TextureData tex = {
		.width = width,
		.height = height,
		.format = format
	};

	Ref<Texture> result = Renderer::create_texture(tex);
	result->set(data);

	return result;
}

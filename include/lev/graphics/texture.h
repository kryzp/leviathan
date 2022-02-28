#pragma once

#include <lev/core/util.h>
#include <lev/graphics/image.h>

namespace Lev
{
	enum class TextureFormat
	{
		RGB,
		RGBA
	};

	struct TextureData
	{
		int width;
		int height;

		TextureFormat format;
	};

	class Texture
	{
	public:
		Texture();
		virtual ~Texture();

		virtual void set(const byte* data) = 0;
		virtual void bind(int i = 0) const = 0;

		virtual TextureData data() const = 0;

		static Ref<Texture> create(const char* path);
		static Ref<Texture> create(const Image& image);
		static Ref<Texture> create(int width, int height, const byte* data, TextureFormat format);

	};
}

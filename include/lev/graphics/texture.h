#pragma once

#include <lev/core/util.h>
#include <lev/math/rect.h>
#include <lev/graphics/image.h>

namespace lev::gfx
{
	enum class TextureFormat
	{
		RGB,
		RGBA
	};

	enum class TextureFilter
	{
		NEAREST,
		LINEAR
	};

	enum class TextureWrap
	{
		CLAMP,
		REPEAT
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

		virtual void generate(const byte* data) = 0;

		virtual const TextureData& data() const = 0;

		static Ref<Texture> create(const char* path);
		static Ref<Texture> create(const Image& image);
		static Ref<Texture> create(int width, int height, const byte* data, TextureFormat format);
	};

	struct TextureSampler
	{
		TextureFilter filter;
		TextureWrap wrap_x;
		TextureWrap wrap_y;
	};

	struct TextureRegion
	{
		Ref<Texture> texture;
		RectI source;
	};
}

#pragma once

#include <lev/core/util.h>
#include <lev/math/rect.h>
#include <lev/graphics/image.h>

namespace lev
{
	class Texture;

	enum class TextureFormat
	{
		RED,
		RG,
		RGB,
		RGBA,
		DEPTH_STENCIL
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

	struct TextureSampler
	{
		TextureFilter filter;
		TextureWrap wrap_x;
		TextureWrap wrap_y;

		static const TextureSampler& pixel();
		static const TextureSampler& linear();
	};

	struct TextureRegion
	{
		Ref<Texture> source;
		RectI bounds;
	};

	class Texture
	{
	public:
		Texture();
		virtual ~Texture();

		static Ref<Texture> create(const char* path);
		static Ref<Texture> create(const Image& image);
		static Ref<Texture> create(int width, int height, TextureFormat format, const byte* data);

		virtual void bind(int i) const = 0;
		virtual void generate(const byte* data) = 0;
		virtual int width() const = 0;
		virtual int height() const = 0;
		virtual TextureFormat format() const = 0;
	};
}

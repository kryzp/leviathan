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

		TextureData()
			: width(0)
			, height(0)
			, format(TextureFormat::RGBA)
		{
		}

		TextureData(int width, int height, TextureFormat format)
			: width(width)
			, height(height)
			, format(format)
		{
		}
	};

	struct TextureSampler
	{
		TextureFilter filter;
		TextureWrap wrap_x;
		TextureWrap wrap_y;

		TextureSampler()
			: filter(TextureFilter::NEAREST)
			, wrap_x(TextureWrap::CLAMP)
			, wrap_y(TextureWrap::CLAMP)
		{
		}

		TextureSampler(TextureFilter filter, TextureWrap wrap_x, TextureWrap wrap_y)
			: filter(filter)
			, wrap_x(wrap_x)
			, wrap_y(wrap_y)
		{
		}

		static const TextureSampler& pixel();
		static const TextureSampler& linear();
	};

	struct TextureRegion
	{
		Ref<Texture> source;
		RectF bounds;

		TextureRegion(const Ref<Texture>& source, const RectF& bounds)
			: source(source)
			, bounds(bounds)
		{
		}
	};

	class Texture
	{
	public:
		Texture();
		virtual ~Texture();

		static Ref<Texture> create(const char* path);
		static Ref<Texture> create(const Image& image);
		static Ref<Texture> create(int width, int height, TextureFormat format, const byte* data);

		Int2 size() const { return Int2(width(), height()); }

		virtual void bind(int i) const = 0;
		virtual void generate(const byte* data) = 0;
		virtual int width() const = 0;
		virtual int height() const = 0;
		virtual TextureFormat format() const = 0;
	};
}

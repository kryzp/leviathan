#pragma once

#include <lev/core/util.h>
#include <lev/math/rect.h>
#include <lev/graphics/image.h>

namespace Lev
{
	enum class TextureFormat
	{
		RGB,
		RGBA
	};

	enum class TextureFilter
	{
		None,
		Nearest,
		Linear
	};

	enum class TextureWrap
	{
		None,
		Clamp,
		Repeat
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

		virtual void set_data(const byte* data) = 0;
		virtual void bind(int i = 0) const = 0;

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

		TextureSampler()
			: filter(TextureFilter::Nearest)
			, wrap_x(TextureWrap::Clamp)
			, wrap_y(TextureWrap::Clamp)
		{
		}

		TextureSampler(TextureFilter filter, TextureWrap wrap_x, TextureWrap wrap_y)
			: filter(filter)
			, wrap_x(wrap_x)
			, wrap_y(wrap_y)
		{
		}

		bool operator == (const TextureSampler& other) const
		{
			return (
				(this->filter == other.filter) &&
				(this->wrap_x == other.wrap_x) &&
				(this->wrap_y == other.wrap_y)
			);
		}

		bool operator != (const TextureSampler& other) const
		{
			return !(*this == other);
		}
	};

	struct TextureRegion
	{
		Ref<Texture> texture;
		RectI source;
	};
}

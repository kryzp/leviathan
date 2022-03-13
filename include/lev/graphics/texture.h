#pragma once

#include <lev/core/util.h>
#include <lev/math/rect.h>
#include <lev/graphics/image.h>

namespace lev::gfx
{
	class Texture;

	enum class TextureFormat
	{
		R,
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
		Ref<Texture> texture;
		RectI source;
	};

	class Texture
	{
	public:
		Texture();
		virtual ~Texture();

		static Ref<Texture> create(const char* path);
		static Ref<Texture> create(const Image& image);
		static Ref<Texture> create(int width, int height, TextureFormat format, const byte* data = nullptr);

		virtual void bind(int i) const = 0;
		virtual void generate(const byte* data) = 0;
		virtual int width() const = 0;
		virtual int height() const = 0;
		virtual TextureFormat format() const = 0;
	};
}

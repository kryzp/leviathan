#pragma once

#include <lev/core/util.h>
#include <lev/math/rect.h>
#include <lev/graphics/image.h>

namespace lev
{
	class Texture;

	enum TextureFormat
	{
		TEX_FMT_RED,
		TEX_FMT_RG,
		TEX_FMT_RGB,
		TEX_FMT_RGBA,
		TEX_FMT_DEPTH_STENCIL,
		TEX_FMT_MAX
	};

    enum InternalTextureFormat
    {
        TEX_I_FMT_R8,
        TEX_I_FMT_R8_SNORM,
        TEX_I_FMT_R16,
        TEX_I_FMT_R16_SNORM,
        TEX_I_FMT_RG8,
        TEX_I_FMT_RG8_SNORM,
        TEX_I_FMT_RG16,
        TEX_I_FMT_RG16_SNORM,
        TEX_I_FMT_R3_G3_B2,
        TEX_I_FMT_RGB4,
        TEX_I_FMT_RGB5,
        TEX_I_FMT_RGB8,
        TEX_I_FMT_RGB8_SNORM,
        TEX_I_FMT_RGB10,
        TEX_I_FMT_RGB12,
        TEX_I_FMT_RGB16_SNORM,
        TEX_I_FMT_RGBA2,
        TEX_I_FMT_RGBA4,
        TEX_I_FMT_RGB5_A1,
        TEX_I_FMT_RGBA8,
        TEX_I_FMT_RGBA8_SNORM,
        TEX_I_FMT_RGB10_A2,
        TEX_I_FMT_RGB10_A2UI,
        TEX_I_FMT_RGBA12,
        TEX_I_FMT_RGBA16,
        TEX_I_FMT_SRGB8,
        TEX_I_FMT_SRGB8_ALPHA8,
        TEX_I_FMT_R16F,
        TEX_I_FMT_RG16F,
        TEX_I_FMT_RGB16F,
        TEX_I_FMT_RGBA16F,
        TEX_I_FMT_R32F,
        TEX_I_FMT_RG32F,
        TEX_I_FMT_RGB32F,
        TEX_I_FMT_RGBA32F,
        TEX_I_FMT_R11F_G11F_B10F,
        TEX_I_FMT_RGB9_E5,
        TEX_I_FMT_R8I,
        TEX_I_FMT_R8UI,
        TEX_I_FMT_R16I,
        TEX_I_FMT_R16UI,
        TEX_I_FMT_R32I,
        TEX_I_FMT_R32UI,
        TEX_I_FMT_RG8I,
        TEX_I_FMT_RG8UI,
        TEX_I_FMT_RG16I,
        TEX_I_FMT_RG16UI,
        TEX_I_FMT_RG32I,
        TEX_I_FMT_RG32UI,
        TEX_I_FMT_RGB8I,
        TEX_I_FMT_RGB8UI,
        TEX_I_FMT_RGB16I,
        TEX_I_FMT_RGB16UI,
        TEX_I_FMT_RGB32I,
        TEX_I_FMT_RGB32UI,
        TEX_I_FMT_RGBA8I,
        TEX_I_FMT_RGBA8UI,
        TEX_I_FMT_RGBA16I,
        TEX_I_FMT_RGBA16UI,
        TEX_I_FMT_RGBA32I,
        TEX_I_FMT_RGBA32UI,
        TEX_I_FMT_MAX
    };

	enum TextureType
	{
        TEX_TYPE_UNSIGNED_BYTE,
        TEX_TYPE_BYTE,
        TEX_TYPE_UNSIGNED_SHORT,
        TEX_TYPE_SHORT,
        TEX_TYPE_UNSIGNED_INT,
        TEX_TYPE_UNSIGNED_INT_24_8,
        TEX_TYPE_INT,
        TEX_TYPE_HALF_FLOAT,
        TEX_TYPE_FLOAT,
        TEX_TYPE_MAX
	};

	enum TextureFilter
	{
		TEX_FILTER_NEAREST,
		TEX_FILTER_LINEAR,
		TEX_FILTER_MAX
	};

	enum TextureWrap
	{
		TEX_WRAP_CLAMP,
		TEX_WRAP_REPEAT,
		TEX_WRAP_MAX
	};

	struct TextureSampler
	{
		u8 filter;
		u8 wrap_x;
		u8 wrap_y;

		TextureSampler()
			: filter(TEX_FILTER_NEAREST)
			, wrap_x(TEX_WRAP_CLAMP)
			, wrap_y(TEX_WRAP_CLAMP)
		{
		}

		TextureSampler(u8 filter, u8 wrap_x, u8 wrap_y)
			: filter(filter)
			, wrap_x(wrap_x)
			, wrap_y(wrap_y)
		{
		}

		static const TextureSampler& pixel();
		static const TextureSampler& linear();
	};

    struct TextureData
    {
        u32 width;
        u32 height;
        u8 format;
        u8 internal_format;
        u8 type;

        TextureData()
            : width(0)
            , height(0)
            , format(TEX_FMT_RGBA)
            , internal_format(TEX_I_FMT_RGBA32F)
            , type(TEX_TYPE_UNSIGNED_BYTE)
        {
        }

        TextureData(u32 width, u32 height, u8 format, u8 internal_format, u8 type)
            : width(width)
            , height(height)
            , format(format)
            , internal_format(internal_format)
            , type(type)
        {
        }
    };

	class Texture
	{
	public:
		Texture() = default;
		virtual ~Texture() = default;

		static Ref<Texture> create(const char* path);
		static Ref<Texture> create(const Image& image, u8 format, u8 internal_format, u8 type);
		static Ref<Texture> create(u32 width, u32 height, u8 format, u8 internal_format, u8 type, const byte* data);

		static void unbind();
		static void unbind_image();

		inline Int2 size() const { return Int2(width(), height()); }

		virtual void copy_to(const Ref<Texture>& other) = 0;

		virtual void bind(int idx) const = 0;
		virtual void bind_image(int idx) const = 0;

		virtual void generate(const void* data) = 0;
		virtual void get_data(float* buf) = 0;

		virtual u32 width() const = 0;
		virtual u32 height() const = 0;

		virtual u8 format() const = 0;
        virtual u8 internal_format() const = 0;
        virtual u8 texture_type() const = 0;
	};

	struct TextureRegion
	{
		Ref<Texture> source;
		RectI bounds;

		TextureRegion()
			: source(nullptr)
			, bounds(RectI::zero())
		{
		}

		TextureRegion(const Ref<Texture>& source, const RectI& bounds = RectI::zero())
			: source(source)
			, bounds(bounds)
		{
			if (bounds == RectI::zero())
				this->bounds = RectI(source->width(), source->height());
		}
	};

	class ArrayTexture
	{
	public:
		ArrayTexture() = default;
		virtual ~ArrayTexture() = default;

		static Ref<ArrayTexture> create(const char* path, u32 image_count);
		static Ref<ArrayTexture> create(const Image& image, u8 format, u8 internal_format, u8 type, u32 image_count);
		static Ref<ArrayTexture> create(u32 width, u32 height, u8 format, u8 internal_format, u8 type, u32 image_count, const byte* data);

		static void unbind();

		virtual void bind(int idx) const = 0;

		virtual void generate(const void* data) = 0;

		virtual u32 width() const = 0;
		virtual u32 height() const = 0;
		virtual u32 image_count() const = 0;

		virtual u8 format() const = 0;
		virtual u8 internal_format() const = 0;
		virtual u8 texture_type() const = 0;
	};
}

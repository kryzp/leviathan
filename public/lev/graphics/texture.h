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
        I_TEX_FMT_R8,
        I_TEX_FMT_R8_SNORM,
        I_TEX_FMT_R16,
        I_TEX_FMT_R16_SNORM,
        I_TEX_FMT_RG8,
        I_TEX_FMT_RG8_SNORM,
        I_TEX_FMT_RG16,
        I_TEX_FMT_RG16_SNORM,
        I_TEX_FMT_R3_G3_B2,
        I_TEX_FMT_RGB4,
        I_TEX_FMT_RGB5,
        I_TEX_FMT_RGB8,
        I_TEX_FMT_RGB8_SNORM,
        I_TEX_FMT_RGB10,
        I_TEX_FMT_RGB12,
        I_TEX_FMT_RGB16_SNORM,
        I_TEX_FMT_RGBA2,
        I_TEX_FMT_RGBA4,
        I_TEX_FMT_RGB5_A1,
        I_TEX_FMT_RGBA8,
        I_TEX_FMT_RGBA8_SNORM,
        I_TEX_FMT_RGB10_A2,
        I_TEX_FMT_RGB10_A2UI,
        I_TEX_FMT_RGBA12,
        I_TEX_FMT_RGBA16,
        I_TEX_FMT_SRGB8,
        I_TEX_FMT_SRGB8_ALPHA8,
        I_TEX_FMT_R16F,
        I_TEX_FMT_RG16F,
        I_TEX_FMT_RGB16F,
        I_TEX_FMT_RGBA16F,
        I_TEX_FMT_R32F,
        I_TEX_FMT_RG32F,
        I_TEX_FMT_RGB32F,
        I_TEX_FMT_RGBA32F,
        I_TEX_FMT_R11F_G11F_B10F,
        I_TEX_FMT_RGB9_E5,
        I_TEX_FMT_R8I,
        I_TEX_FMT_R8UI,
        I_TEX_FMT_R16I,
        I_TEX_FMT_R16UI,
        I_TEX_FMT_R32I,
        I_TEX_FMT_R32UI,
        I_TEX_FMT_RG8I,
        I_TEX_FMT_RG8UI,
        I_TEX_FMT_RG16I,
        I_TEX_FMT_RG16UI,
        I_TEX_FMT_RG32I,
        I_TEX_FMT_RG32UI,
        I_TEX_FMT_RGB8I,
        I_TEX_FMT_RGB8UI,
        I_TEX_FMT_RGB16I,
        I_TEX_FMT_RGB16UI,
        I_TEX_FMT_RGB32I,
        I_TEX_FMT_RGB32UI,
        I_TEX_FMT_RGBA8I,
        I_TEX_FMT_RGBA8UI,
        I_TEX_FMT_RGBA16I,
        I_TEX_FMT_RGBA16UI,
        I_TEX_FMT_RGBA32I,
        I_TEX_FMT_RGBA32UI,
        I_TEX_FMT_MAX
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
            , internal_format(I_TEX_FMT_RGBA32F)
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

		static Texture* create(const char* path);
		static Texture* create(const Image& image, u8 format, u8 internal_format, u8 type);
		static Texture* create(u32 width, u32 height, u8 format, u8 internal_format, u8 type, const byte* data);

		static void unbind();
		static void unbind_image();

		inline Int2 size() const { return Int2(width(), height()); }

		virtual void copy_to(Ref<Texture>& other) = 0;

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
        Texture* source;
        RectI bounds;

        TextureRegion()
            : source(nullptr)
            , bounds(RectI::zero())
        {
        }

        TextureRegion(Texture* source, const RectI& bounds = RectI::zero())
            : source(source)
            , bounds(bounds)
        {
            if (bounds == RectI::zero())
                this->bounds = RectI(source->width(), source->height());
        }
    };
}

#pragma once

#include <lev/core/util.h>
#include <lev/math/rect.h>
#include <lev/graphics/image.h>

namespace lev
{
	class Texture;

	enum TextureFormat
	{
		TEX_FMT_NONE = 0,
		TEX_FMT_RED,
		TEX_FMT_RG,
		TEX_FMT_RGB,
		TEX_FMT_RGBA,
		TEX_FMT_DEPTH_STENCIL,
		TEX_FMT_MAX
	};

    enum InternalTextureFormat
    {
		TEX_I_FMT_NONE = 0,
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
		TEX_TYPE_NONE = 0,
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
		TEX_FILTER_NONE = 0,
		TEX_FILTER_NEAREST,
		TEX_FILTER_LINEAR,
		TEX_FILTER_MAX
	};

	enum TextureWrap
	{
		TEX_WRAP_NONE = 0,
		TEX_WRAP_CLAMP,
		TEX_WRAP_REPEAT,
		TEX_WRAP_MAX
	};

	struct TextureSampler
	{
		TextureFilter filter;
		TextureWrap wrap_x;
		TextureWrap wrap_y;

		TextureSampler()
			: filter(TEX_FILTER_NONE)
			, wrap_x(TEX_WRAP_NONE)
			, wrap_y(TEX_WRAP_NONE)
		{
		}

		TextureSampler(TextureFilter filter, TextureWrap wrap_x, TextureWrap wrap_y)
			: filter(filter)
			, wrap_x(wrap_x)
			, wrap_y(wrap_y)
		{
		}

		bool operator == (const TextureSampler& other) const { return this->filter == other.filter && this->wrap_x == other.wrap_x && this->wrap_y == other.wrap_y; }
		bool operator != (const TextureSampler& other) const { return !(*this == other); }

		static const TextureSampler& pixel();
		static const TextureSampler& linear();
	};

	struct TextureFormatInfo
	{
		TextureFormat format;
		InternalTextureFormat internal;
		TextureType type;

		TextureFormatInfo(TextureFormat format, InternalTextureFormat internal, TextureType type)
			: format(format)
			, internal(internal)
			, type(type)
		{
		}
	};

	class Texture : public NonCopyable, public NonMovable
	{
	public:
		Texture() = default;
		virtual ~Texture() = default;

		static Ref<Texture> create(const char* path);
		static Ref<Texture> create(const Image& image, TextureFormat format, InternalTextureFormat internal_format, TextureType type);
		static Ref<Texture> create(u32 width, u32 height, TextureFormat format, InternalTextureFormat internal_format, TextureType type, const byte* data);

		static Ref<Texture> create_array(const char* path, u32 image_count);
		static Ref<Texture> create_array(const Image& image, TextureFormat format, InternalTextureFormat internal_format, TextureType type, u32 image_count);
		static Ref<Texture> create_array(u32 width, u32 height, TextureFormat format, InternalTextureFormat internal_format, TextureType type, u32 image_count, const byte* data);

		static void unbind();
		static void unbind_array();
		static void unbind_image();

		inline Int2 size() const { return Int2(width(), height()); }

		virtual void copy_to(const Ref<Texture>& other) = 0;

		virtual void bind(int idx) const = 0;
		virtual void bind_image(int idx) const = 0;

		virtual void generate(const void* data) = 0;
		virtual void get_data(float* buf) = 0;

		virtual bool framebuffer_parent() = 0;

		virtual u32 width() const = 0;
		virtual u32 height() const = 0;

		virtual u32 array_depth() const = 0;
		virtual bool is_array_texture() const = 0;

		virtual TextureFormatInfo format_info() const = 0;
	};
}

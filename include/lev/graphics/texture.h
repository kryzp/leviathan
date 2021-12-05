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

	class Texture
	{
	public:
		Texture();
		~Texture();

		void set(const byte* data);
		void bind(int i = 0) const;
		void free() const;

		int width() const;
		int height() const;

		static Ref<Texture> create(const char* path);
		static Ref<Texture> create(const Image& image);
		static Ref<Texture> create(int width, int height, const byte* data, TextureFormat format);

	private:
		u32 m_id;
		int m_width;
		int m_height;

	};
}

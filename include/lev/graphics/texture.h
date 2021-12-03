#pragma once

#include <lev/core/util.h>
#include <lev/graphics/image.h>

namespace Lev
{
	class Texture
	{
	public:
		Texture();

		Texture(const Ref<Image>& image);
		Texture(const char* path);
		
		~Texture();

		void load(const Ref<Image>& image);
		void load(const char* path);

		void bind(int i = 0) const;
		void free() const;

	private:
		u32 m_id;

	};
}

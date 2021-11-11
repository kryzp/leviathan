#pragma once

#include <lev/core/util.h>

namespace Lev
{
	class Texture;
	using TextureRef = Ref<Texture>;

	class Texture
	{
	public:
		Texture();
		Texture(const char* path);
		~Texture();

		void load(const char* path);
		void bind(int i) const;
		void free() const;

	private:
		u32 m_id;

	};
}

#pragma once

#include <lev/graphics/texture.h>
#include <lev/graphics/shader.h>

namespace Lev
{
	class SpriteBatch
	{
	public:
		SpriteBatch();
		~SpriteBatch();

		void render(int x, int y, const Ref<Texture>& texture);

		void set_shader(const Ref<Shader>& shader);

	private:
	};
}

#pragma once

#include <lev/node/2d/node_2d.h>
#include <lev/graphics/sprite_batch.h>
#include <lev/graphics/texture.h>

/*
crap class to test out node system
*/

namespace lev
{
	class Sprite2D : public Node2D
	{
	public:
		Sprite2D(const TextureRegion& texture)
			: m_texture(texture)
		{
		}

		void render(SpriteBatch& b) override
		{
			b.push_matrix(matrix_relative_to_parent(parent()));
			b.push_texture(m_texture);
			b.pop_matrix();
		}

	private:
		TextureRegion m_texture;
	};
}

#pragma once

#include <lev/graphics/sprite_batch.h>
#include <lev/graphics/texture.h>

#include "../ui_component.h"

namespace lev
{
	class UITextureComponent : public UIComponent
	{
	public:
		gfx::TextureRegion texture;

		UITextureComponent(const gfx::TextureRegion& tex)
			: UIComponent()
			, texture(tex)
		{
		}

		~UITextureComponent()
		{
		}

		void render(gfx::SpriteBatch& b) override
		{
			UIComponent::render(b);

			auto texsize = Vec2(texture.texture->width(), texture.texture->height());

			b.push_matrix(Mat3x2::create_transform(
				position(),
				0.0f,
				size() / texsize,
				Vec2F::zero()
			));

			b.render_texture(texture.texture);

			b.pop_matrix();
		}
	};
};

#pragma once

#include <lev/graphics/sprite_batch.h>
#include <lev/ui/ui_component.h>
#include <lev/graphics/texture.h>

namespace Lev
{
	class UITextureComponent : public UIComponent
	{
	public:
		TextureRegion texture;

		UITextureComponent(const TextureRegion& tex)
			: UIComponent()
			, texture(tex)
		{
		}

		~UITextureComponent()
		{
		}

		void render(SpriteBatch& b) override
		{
			UIComponent::render(b);

			auto data = texture.texture->data();
			auto texsize = Vec2(data.width, data.height);

			b.push_matrix(Mat3x2::create_transform(
				position(),
				0.0f,
				size() / texsize,
				Vec2::ZERO
			));

			b.render_texture(texture);

			b.pop_matrix();
		}
	};
};

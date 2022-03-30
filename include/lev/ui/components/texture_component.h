#pragma once

#include <lev/ui/ui_component.h>

#include <lev/graphics/sprite_batch.h>
#include <lev/graphics/texture.h>

namespace lev
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

		void render(SpriteBatch& b) override
		{
			UIComponent::render(b);

			b.push_matrix(Mat3x2::create_transform(position(), 0.0f, size() / texture.source->size(), Vec2F::zero()));
			b.push_texture(texture);
			b.pop_matrix();
		}
	};
};

#pragma once

#include <lev/ui/ui_component.h>

#include <lev/graphics/sprite_batch.h>
#include <lev/graphics/texture.h>

#include <functional>

namespace lev
{
	class UIClickableTexture : public UIComponent
	{
	public:
		TextureRegion texture;
		std::function<void(void)> on_clicked;

		UIClickableTexture(const TextureRegion& tex, const std::function<void(void)>& on_clicked = nullptr)
			: UIComponent()
			, texture(tex)
			, on_clicked(on_clicked)
		{
		}

		void update() override
		{
			if (mouse_clicked_over() && on_clicked)
				on_clicked();
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

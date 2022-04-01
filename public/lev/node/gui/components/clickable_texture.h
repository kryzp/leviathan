#pragma once

#include <lev/node/gui/gui_component.h>
#include <lev/graphics/sprite_batch.h>
#include <lev/graphics/texture.h>
#include <lev/input/input.h>
#include <lev/math/vec2.h>
#include <functional>

/*
 * just for demonstration of how to use / testing
 */

namespace lev
{
	class GUIClickableTexture : public GUINode
	{
	public:
		struct ClickData
		{
			Point2 where;
		};

		using ClickFn = std::function<void(ClickData)>;

		TextureRegion texture;
		ClickFn on_clicked;

		GUIClickableTexture(const GUIConstraints& constraints, const TextureRegion& texture, const ClickFn& on_clicked)
			: GUINode(constraints)
			, texture(texture)
			, on_clicked(on_clicked)
		{
		}

		void update() override
		{
			GUINode::update();

			if (mouse_clicked_over())
			{
				ClickData data;
				data.where = Input::mouse_draw_pos() - position();
				on_clicked(data);
			}
		}

		void render(SpriteBatch& b) override
		{
			GUINode::render(b);

			b.push_matrix(Mat3x2::create_transform(position(), 0.0f, size() / texture.source->size(), Vec2F::zero()));
			b.push_texture(texture);
			b.pop_matrix();
		}
	};
}

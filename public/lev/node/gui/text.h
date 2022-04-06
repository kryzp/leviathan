#pragma once

#include <lev/node/gui/gui_node.h>
#include <lev/graphics/sprite_batch.h>
#include <lev/graphics/font.h>

#include <string>

namespace lev
{
	class GUIText : public GUINode
	{
	public:
		Ref<Font> font;
		std::string text;

		GUIText(const GUIConstraints& constraints, const std::string& text, const Ref<Font>& font)
			: GUINode(constraints)
			, font(font)
			, text(text)
		{
		}

		void render(SpriteBatch& b) override
		{
			GUINode::render(b);

			b.push_matrix(Mat3x2::create_transform(position(), 0.0f, size(), Vec2F::zero()));
			b.push_string(text.c_str(), font, TEXT_ALIGN_LEFT, Colour::white());
			b.pop_matrix();
		}
	};
}

#pragma once

#include <lev/node/gui/gui_node.h>
#include <lev/graphics/sprite_batch.h>
#include <lev/graphics/font.h>

#include <string>

/*
 * just for demonstration of how to use / testing
 */

namespace lev
{
	class GUIText : public GUINode
	{
	public:
		const Font& font;
		std::string text;

		GUIText(const GUIConstraints& constraints, const std::string& text, const Font& font)
			: GUINode(constraints)
			, font(font)
			, text(text)
		{
		}

		void render(SpriteBatch& b) override
		{
			GUINode::render(b);

			b.push_matrix(Mat3x2::create_transform(position(), 0.0f, size(), Vec2F::zero()));
			b.push_text(text.c_str(), font);
			b.pop_matrix();
		}
	};
}

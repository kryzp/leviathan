#pragma once

#include <lev/gui/gui_component.h>
#include <lev/graphics/sprite_batch.h>
#include <lev/graphics/font.h>
#include <lev/containers/string.h>

namespace lev
{
	class GUIText : public GUIComponent
	{
	public:
		Ref<Font> font;
		String text;

		GUIText(const GUIConstraints& constraints, const String& text, const Ref<Font>& font)
			: GUIComponent(constraints)
			, font(font)
			, text(text)
		{
		}

		void render(SpriteBatch& b) override
		{
			GUIComponent::render(b);

			b.push_matrix(Mat3x2::create_transform(position(), 0.0f, size(), Vec2F::zero()));
			b.push_string(text.c_str(), font, TEXT_ALIGN_LEFT, Colour::white());
			b.pop_matrix();
		}
	};
}

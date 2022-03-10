#pragma once

#include "../ui_constraints.h"

namespace lev::ui
{
	class PixelConstraint : public UIConstraint
	{
	public:
		int pixels;

		PixelConstraint(int px)
			: pixels(px)
		{
		}

		int constrain_x(UIComponent& component) { return pixels; }
		int constrain_y(UIComponent& component) { return pixels; }
		int constrain_width(UIComponent& component) { return pixels; }
		int constrain_height(UIComponent& component) { return pixels; }
	};
};

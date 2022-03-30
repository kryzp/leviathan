#pragma once

#include <lev/ui/ui_constraints.h>

namespace lev
{
	class UIPixelConstraint : public UIConstraint
	{
	public:
		int pixels;

		UIPixelConstraint(int px)
			: UIConstraint()
			, pixels(px)
		{
		}

		int constrain_x(const UIComponent& component) override { return pixels; }
		int constrain_y(const UIComponent& component) override { return pixels; }
		int constrain_width(const UIComponent& component) override { return pixels; }
		int constrain_height(const UIComponent& component) override { return pixels; }
	};
};

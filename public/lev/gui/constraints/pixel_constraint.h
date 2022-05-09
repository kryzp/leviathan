#pragma once

#include <lev/gui/gui_constraints.h>

namespace lv
{
	class PixelConstraint : public GUIConstraint
	{
	public:
		int pixels;

		PixelConstraint(int px)
			: GUIConstraint()
			, pixels(px)
		{
		}

		int constrain_x(const GUIComponent& component)		const override { return pixels; }
		int constrain_y(const GUIComponent& component)		const override { return pixels; }
		int constrain_width(const GUIComponent& component)	const override { return pixels; }
		int constrain_height(const GUIComponent& component)	const override { return pixels; }
	};
}

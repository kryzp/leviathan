#pragma once

#include <lev/node/gui/gui_constraints.h>

namespace lev
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

		int constrain_x(const GUINode& component)		const override { return pixels; }
		int constrain_y(const GUINode& component)		const override { return pixels; }
		int constrain_width(const GUINode& component)	const override { return pixels; }
		int constrain_height(const GUINode& component)	const override { return pixels; }
	};
}

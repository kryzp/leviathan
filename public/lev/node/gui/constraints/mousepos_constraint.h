#pragma once

#include <lev/node/gui/gui_constraints.h>
#include <lev/input/input.h>

namespace lev
{
	class MousePosConstraint : public GUIConstraint
	{
	public:
		MousePosConstraint()
			: GUIConstraint()
		{
		}

		int constrain_x(const GUINode& component)		const override { return Input::mouse_draw_pos().x; }
		int constrain_y(const GUINode& component)		const override { return Input::mouse_draw_pos().y; }
		int constrain_width(const GUINode& component)	const override { return Input::mouse_draw_pos().x; }
		int constrain_height(const GUINode& component)	const override { return Input::mouse_draw_pos().y; }
	};
}

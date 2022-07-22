#pragma once

#include <lev/gui/gui_constraints.h>
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

		int constrain_x(const GUIComponent& component)		const override { return Input::inst()->mouse_draw_pos().x; }
		int constrain_y(const GUIComponent& component)		const override { return Input::inst()->mouse_draw_pos().y; }
		int constrain_width(const GUIComponent& component)	const override { return Input::inst()->mouse_draw_pos().x; }
		int constrain_height(const GUIComponent& component)	const override { return Input::inst()->mouse_draw_pos().y; }
	};
}

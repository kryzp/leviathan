#include <lev/node/gui/gui_constraints.h>
#include <lev/node/gui/gui_component.h>

#include <lev/node/gui/constraints/pixel_constraint.h>
#include <lev/node/gui/constraints/mousepos_constraint.h>

using namespace lev;

GUIConstraints::GUIConstraints()
	: x(nullptr)
	, y(nullptr)
	, width(nullptr)
	, height(nullptr)
{
}

GUIConstraints::~GUIConstraints()
{
}

GUIConstraints GUIConstraints::create_fixed(int x, int y, int width, int height)
{
	GUIConstraints constraints;
	{
		constraints.x      = create_ref<PixelConstraint>(x);
		constraints.y      = create_ref<PixelConstraint>(y);
		constraints.width  = create_ref<PixelConstraint>(width);
		constraints.height = create_ref<PixelConstraint>(height);
	}
	return constraints;
}

GUIConstraints GUIConstraints::create_mousepos(int width, int height)
{
	GUIConstraints constraints;
	{
		constraints.x      = create_ref<MousePosConstraint>();
		constraints.y      = create_ref<MousePosConstraint>();
		constraints.width  = create_ref<PixelConstraint>(width);
		constraints.height = create_ref<PixelConstraint>(height);
	}
	return constraints;
}

void GUIConstraints::constrain(GUIComponent& component)
{
	if (x)
		component.x(x->constrain_x(component));

	if (y)
		component.y(y->constrain_y(component));

	if (width)
		component.width(width->constrain_width(component));

	if (height)
		component.height(height->constrain_height(component));
}

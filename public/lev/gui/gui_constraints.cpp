#include <lev/gui/gui_constraints.h>
#include <lev/gui/gui_component.h>

#include <lev/gui/constraints/pixel_constraint.h>
#include <lev/gui/constraints/mousepos_constraint.h>

using namespace lv;

GUIConstraints GUIConstraints::create_fixed(int x, int y, int w, int h)
{
	GUIConstraints c;

	c.x = create_ref<PixelConstraint>(x);
	c.y = create_ref<PixelConstraint>(y);
	c.w = create_ref<PixelConstraint>(w);
	c.h = create_ref<PixelConstraint>(h);
	
	return c;
}

GUIConstraints GUIConstraints::create_mousepos(int w, int h)
{
	GUIConstraints c;

	c.x = create_ref<MousePosConstraint>();
	c.y = create_ref<MousePosConstraint>();
	c.w = create_ref<PixelConstraint>(w);
	c.h = create_ref<PixelConstraint>(h);
	
	return c;
}

void GUIConstraints::constrain(GUIComponent& component)
{
	if (x)
		component.x(x->constrain_x(component));

	if (y)
		component.y(y->constrain_y(component));

	if (w)
		component.width(w->constrain_width(component));

	if (h)
		component.height(h->constrain_height(component));
}

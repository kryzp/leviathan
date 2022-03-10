#include "ui_constraints.h"
#include "ui_component.h"
#include "constraints/pixel_constraint.h"

using namespace lev;
using namespace lev::ui;

UIConstraints::UIConstraints()
	: x(nullptr)
	, y(nullptr)
	, width(nullptr)
	, height(nullptr)
{
}

UIConstraints::~UIConstraints()
{
}

UIConstraints UIConstraints::create_fixed(int x, int y, int width, int height)
{
	UIConstraints constraints;
	{
		constraints.x = create_ref<PixelConstraint>(x);
		constraints.y = create_ref<PixelConstraint>(y);
		constraints.width = create_ref<PixelConstraint>(width);
		constraints.height = create_ref<PixelConstraint>(height);
	}
	return constraints;
}

void UIConstraints::constrain(UIComponent& component)
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

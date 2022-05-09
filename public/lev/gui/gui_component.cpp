#include <lev/gui/gui_component.h>
#include <lev/input/input.h>

using namespace lv;

GUIComponent::GUIComponent()
	: use_constraints(false)
	, enabled(true)
{
}

GUIComponent::GUIComponent(const GUIConstraints& c)
	: constraints(c)
	, use_constraints(true)
    , enabled(true)
{
	this->constraints.constrain(*this);
}

GUIComponent::~GUIComponent()
{
}

void GUIComponent::init()
{
}

void GUIComponent::update()
{
    if (!enabled)
        return;

	if (use_constraints)
		this->constraints.constrain(*this);
	
	for (auto& n : components)
        n->update();
}

void GUIComponent::render(SpriteBatch& b)
{
    if (!enabled)
        return;

    for (auto& n : components)
        n->render(b);
}

bool GUIComponent::mouse_hovering_over(bool include_children)
{
	Vec2F mouse_pos = Input::inst()->mouse_draw_pos();
	RectI mouse_rect = RectI(mouse_pos.x, mouse_pos.y, 1, 1);

	bool hovering = mouse_rect.intersects(bounding_box());

	if (!hovering && include_children)
	{
		for (auto& child : components)
		{
			RectI bb = child->bounding_box();
			hovering = mouse_rect.intersects(bb);

			if (hovering)
				break;
		}
	}

	return hovering;
}

bool GUIComponent::mouse_clicked_over(bool include_children, int mb)
{
	return mouse_hovering_over(include_children) && Input::inst()->pressed_mb(mb);
}

RectI GUIComponent::bounding_box() const { return m_rect; }

Vec2F GUIComponent::position() const { return Vec2F(x(), y()); }
Vec2F GUIComponent::size() const { return Vec2F(width(), height()); }

int GUIComponent::x() const { return m_rect.x; }
void GUIComponent::x(int x) { m_rect.x = x + (parent ? parent->x() : 0); }

int GUIComponent::y() const { return m_rect.y; }
void GUIComponent::y(int y) { m_rect.y = y + (parent ? parent->y() : 0); }

int GUIComponent::width() const { return m_rect.w; }
void GUIComponent::width(int w) { m_rect.w = w; }

int GUIComponent::height() const { return m_rect.h; }
void GUIComponent::height(int h) { m_rect.h = h; }

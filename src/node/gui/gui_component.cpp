#include <lev/node/gui/gui_component.h>
#include <lev/input/input.h>

using namespace lev;

GUIComponent::GUIComponent(const GUIConstraints& constraints)
	: Node()
	, constraints(constraints)
	, enabled(true)
	, alpha(1.0f)
{
	this->constraints.constrain(*this);
}

GUIComponent::~GUIComponent()
{
	for (auto& component : m_components)
		delete component;
}

void GUIComponent::init()
{
}

void GUIComponent::update()
{
	for (auto& component : m_components)
	{
		if (!component->enabled)
			continue;

		component->constraints.constrain(*component);
		component->update();
	}
}

void GUIComponent::render(SpriteBatch& b)
{
	for (auto& component : m_components)
	{
		if (!component->enabled)
			continue;

		component->render(b);
	}
}

bool GUIComponent::mouse_hovering_over(bool include_children)
{
	Vec2F mouse_pos = Input::mouse_draw_pos();
	RectI mouse_rect = RectI(mouse_pos.x, mouse_pos.y, 1, 1);

	bool hovering = mouse_rect.intersects(bounding_box());

	if (!hovering && include_children)
	{
		for (auto& child : m_components)
		{
			RectI bb = child->bounding_box();
			hovering = mouse_rect.intersects(bb);

			if (hovering)
				break;
		}
	}

	return hovering;
}

bool GUIComponent::mouse_clicked_over(bool include_children)
{
	return mouse_hovering_over(include_children) && mouse_clicked();
}

bool GUIComponent::mouse_clicked()
{
	return Input::pressed_mb(MB_LEFT); // todo: temp
}

void GUIComponent::clear() { m_components.clear(); }
RectI GUIComponent::bounding_box() const { return m_rect; }

Vec2F GUIComponent::position() const { return Vec2F(x(), y()); }
Vec2F GUIComponent::size() const { return Vec2F(width(), height()); }

int GUIComponent::x() const { return m_rect.x; }
void GUIComponent::x(int x) { m_rect.x = x + ((dynamic_cast<GUIComponent*>(parent())) ? static_cast<GUIComponent*>(parent())->x() : 0); } // todo this is so so so horrible please remove or fix or for the love of god

int GUIComponent::y() const { return m_rect.y; }
void GUIComponent::y(int y) { m_rect.y = y + ((dynamic_cast<GUIComponent*>(parent())) ? static_cast<GUIComponent*>(parent())->y() : 0); } // pain

int GUIComponent::width() const { return m_rect.w; }
void GUIComponent::width(int w) { m_rect.w = w; }

int GUIComponent::height() const { return m_rect.h; }
void GUIComponent::height(int h) { m_rect.h = h; }

#include <lev/ui/ui_component.h>
#include <lev/input/input.h>

using namespace Lev;

UIComponent::UIComponent()
	: parent(nullptr)
	, enabled(true)
	, alpha(1.0f)
{
}

UIComponent::~UIComponent()
{
	for (auto& component : m_components)
		delete component;
}

void UIComponent::init()
{
}

void UIComponent::update()
{
	for (auto& component : m_components)
	{
		if (!component->enabled)
			continue;

		component->constraints.constrain(*component);
		component->update();
	}
}

void UIComponent::render(SpriteBatch& b)
{
	for (auto& component : m_components)
	{
		if (!component->enabled)
			continue;

		component->render(b);
	}
}

/*
void UIComponent::add(Ref<UIComponent> component, const UIConstraints& constraints)
{
	component->parent = this;
	component->constraints = constraints;
	component->constraints.constrain(*component);
	component->init();
	m_components.push_back(component);
}
*/

void UIComponent::clear()
{
	m_components.clear();
}

RectI UIComponent::bounding_box() const
{
	return RectI(x(), y(), width(), height());
}

bool UIComponent::mouse_hovering_over(bool include_children)
{
	Vec2 mouse_pos = Input::mouse_screen_pos();
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

bool UIComponent::mouse_clicked_over(bool include_children)
{
	return mouse_hovering_over(include_children) && mouse_clicked();
}

bool UIComponent::mouse_clicked()
{
	return Input::pressed(MouseButton::LEFT); // todo: temp
}

Vec2 UIComponent::position() const { return Vec2(x(), y()); }
Vec2 UIComponent::size() const { return Vec2(width(), height()); }

int UIComponent::x() const { return m_rect.x; }
void UIComponent::x(int x) { m_rect.x = x + ((parent) ? parent->x() : 0); }

int UIComponent::y() const { return m_rect.y; }
void UIComponent::y(int y) { m_rect.y = y + ((parent) ? parent->y() : 0); }

int UIComponent::width() const { return m_rect.w; }
void UIComponent::width(int w) { m_rect.w = w; }

int UIComponent::height() const { return m_rect.h; }
void UIComponent::height(int h) { m_rect.h = h; }

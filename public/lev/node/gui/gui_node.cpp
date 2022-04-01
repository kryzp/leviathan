#include <lev/node/gui/gui_node.h>
#include <lev/input/input.h>

using namespace lev;

GUINode::GUINode(const GUIConstraints& constraints)
	: Node()
	, constraints(constraints)
	, enabled(true)
	, alpha(1.0f)
{
	this->constraints.constrain(*this);
}

GUINode::~GUINode()
{
	for (auto& component : m_components)
		delete component;
}

void GUINode::init()
{
}

void GUINode::update()
{
	this->constraints.constrain(*this);

	for (auto& component : m_components)
	{
		if (!component->enabled)
			continue;

		component->constraints.constrain(*component);
		component->update();
	}
}

void GUINode::render(SpriteBatch& b)
{
	for (auto& component : m_components)
	{
		if (!component->enabled)
			continue;

		component->render(b);
	}
}

bool GUINode::mouse_hovering_over(bool include_children)
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

bool GUINode::mouse_clicked_over(bool include_children)
{
	return mouse_hovering_over(include_children) && mouse_clicked();
}

bool GUINode::mouse_clicked()
{
	return Input::pressed_mb(MB_LEFT); // todo: temp
}

void GUINode::clear() { m_components.clear(); }
RectI GUINode::bounding_box() const { return m_rect; }

Vec2F GUINode::position() const { return Vec2F(x(), y()); }
Vec2F GUINode::size() const { return Vec2F(width(), height()); }

int GUINode::x() const { return m_rect.x; }
void GUINode::x(int x) { m_rect.x = x + ((dynamic_cast<GUINode*>(parent())) ? static_cast<GUINode*>(parent())->x() : 0); } // todo this is so so so horrible please remove or fix or for the love of god

int GUINode::y() const { return m_rect.y; }
void GUINode::y(int y) { m_rect.y = y + ((dynamic_cast<GUINode*>(parent())) ? static_cast<GUINode*>(parent())->y() : 0); } // pain

int GUINode::width() const { return m_rect.w; }
void GUINode::width(int w) { m_rect.w = w; }

int GUINode::height() const { return m_rect.h; }
void GUINode::height(int h) { m_rect.h = h; }

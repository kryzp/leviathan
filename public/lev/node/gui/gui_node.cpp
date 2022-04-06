#include <lev/node/gui/gui_node.h>
#include <lev/input/input.h>

using namespace lev;

GUINode::GUINode(const GUIConstraints& c)
	: Node()
	, constraints(c)
{
	this->constraints.constrain(*this);
}

GUINode::~GUINode()
{
}

void GUINode::init()
{
	Node::init();
}

void GUINode::update()
{
	this->constraints.constrain(*this);
	
	for (auto& n : children())
	{
		auto guin = static_cast<GUINode*>(n);
		guin->constraints.constrain(*guin);
	}

	Node::update();
}

void GUINode::render(SpriteBatch& b)
{
	Node::render(b);
}

bool GUINode::mouse_hovering_over(bool include_children)
{
	Vec2F mouse_pos = Input::inst()->mouse_draw_pos();
	RectI mouse_rect = RectI(mouse_pos.x, mouse_pos.y, 1, 1);

	bool hovering = mouse_rect.intersects(bounding_box());

	if (!hovering && include_children)
	{
		for (auto& n : children())
		{
			auto child = static_cast<GUINode*>(n);
			RectI bb = child->bounding_box();
			hovering = mouse_rect.intersects(bb);

			if (hovering)
				break;
		}
	}

	return hovering;
}

bool GUINode::mouse_clicked_over(bool include_children, int mb)
{
	return mouse_hovering_over(include_children) && Input::inst()->pressed_mb(mb);
}

RectI GUINode::bounding_box() const { return m_rect; }

Vec2F GUINode::position() const { return Vec2F(x(), y()); }
Vec2F GUINode::size() const { return Vec2F(width(), height()); }

int GUINode::x() const { return m_rect.x; }
void GUINode::x(int x) { m_rect.x = x + ((dynamic_cast<GUINode*>(parent())) ? static_cast<GUINode*>(parent())->x() : 0); } // hmmmm

int GUINode::y() const { return m_rect.y; }
void GUINode::y(int y) { m_rect.y = y + ((dynamic_cast<GUINode*>(parent())) ? static_cast<GUINode*>(parent())->y() : 0); } // hmmmmmmmm

int GUINode::width() const { return m_rect.w; }
void GUINode::width(int w) { m_rect.w = w; }

int GUINode::height() const { return m_rect.h; }
void GUINode::height(int h) { m_rect.h = h; }

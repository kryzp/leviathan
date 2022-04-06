#pragma once

#include <lev/node/node.h>
#include <lev/node/gui/gui_constraints.h>
#include <lev/math/vec2.h>
#include <lev/math/rect.h>
#include <lev/input/input.h>

namespace lev
{
	class GUINode : public Node
	{
	public:
		GUINode(const GUIConstraints& c);
		virtual ~GUINode();

		virtual void init();
		virtual void update();
		virtual void render(SpriteBatch& b);

		virtual bool mouse_hovering_over(bool include_children = true);
		virtual bool mouse_clicked_over(bool include_children = true, int mb = MB_LEFT);

		RectI bounding_box() const;

		Vec2F position() const;
		Vec2F size() const;

		int x() const;
		void x(int x);

		int y() const;
		void y(int y);

		int width() const;
		void width(int w);

		int height() const;
		void height(int h);

		GUIConstraints constraints;

	protected:
		RectI m_rect;
	};
}

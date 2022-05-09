#pragma once

#include <lev/core/util.h>

namespace lv
{
	class GUIComponent;

	class GUIConstraint
	{
	public:
		GUIConstraint() = default;
		virtual ~GUIConstraint() = default;

		virtual int constrain_x(const GUIComponent& component) const = 0;
		virtual int constrain_y(const GUIComponent& component) const = 0;
		virtual int constrain_width(const GUIComponent& component) const = 0;
		virtual int constrain_height(const GUIComponent& component) const = 0;
	};

	class GUIConstraints
	{
	public:
		GUIConstraints() = default;
		~GUIConstraints() = default;

		static GUIConstraints create_fixed(int x, int y, int w, int h);
		static GUIConstraints create_mousepos(int w, int h);

		void constrain(GUIComponent& component);

		Ref<GUIConstraint> x;
		Ref<GUIConstraint> y;
		Ref<GUIConstraint> w;
		Ref<GUIConstraint> h;
	};
}

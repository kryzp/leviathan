#pragma once

#include <lev/core/util.h>

namespace lev
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
		GUIConstraints();
		~GUIConstraints();

		void constrain(GUIComponent& component);

		static GUIConstraints create_fixed(int x, int y, int width, int height);
		static GUIConstraints create_mousepos(int width, int height);

		Ref<GUIConstraint> x;
		Ref<GUIConstraint> y;
		Ref<GUIConstraint> width;
		Ref<GUIConstraint> height;
	};
}

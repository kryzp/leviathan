#pragma once

#include <lev/core/util.h>

namespace lev
{
	class GUINode;

	class GUIConstraint
	{
	public:
		GUIConstraint() = default;
		virtual ~GUIConstraint() = default;

		virtual int constrain_x(const GUINode& component) const = 0;
		virtual int constrain_y(const GUINode& component) const = 0;
		virtual int constrain_width(const GUINode& component) const = 0;
		virtual int constrain_height(const GUINode& component) const = 0;
	};

	class GUIConstraints
	{
	public:
		GUIConstraints() = default;
		~GUIConstraints() = default;

		static GUIConstraints create_fixed(int x, int y, int w, int h);
		static GUIConstraints create_mousepos(int w, int h);

		void constrain(GUINode& component);

		Ref<GUIConstraint> x;
		Ref<GUIConstraint> y;
		Ref<GUIConstraint> w;
		Ref<GUIConstraint> h;
	};
}

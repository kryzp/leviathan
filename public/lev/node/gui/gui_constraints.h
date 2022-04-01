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
		GUIConstraints();
		~GUIConstraints();

		void constrain(GUINode& component);

		static GUIConstraints create_fixed(int x, int y, int width, int height);
		static GUIConstraints create_mousepos(int width, int height);

		Ref<GUIConstraint> x;
		Ref<GUIConstraint> y;
		Ref<GUIConstraint> width;
		Ref<GUIConstraint> height;
	};
}

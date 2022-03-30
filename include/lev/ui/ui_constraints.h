#pragma once

#include <lev/core/util.h>
#include <functional>

namespace lev
{
	class UIComponent;

	class UIConstraint
	{
	public:
		UIConstraint() = default;
		virtual ~UIConstraint() = default;

		virtual int constrain_x(const UIComponent& component) = 0;
		virtual int constrain_y(const UIComponent& component) = 0;
		virtual int constrain_width(const UIComponent& component) = 0;
		virtual int constrain_height(const UIComponent& component) = 0;
	};

	class UIConstraints
	{
	public:
		UIConstraints();
		~UIConstraints();

		void constrain(UIComponent& component);

		static UIConstraints create_fixed(int x, int y, int width, int height);

		Ref<UIConstraint> x;
		Ref<UIConstraint> y;
		Ref<UIConstraint> width;
		Ref<UIConstraint> height;
	};
}

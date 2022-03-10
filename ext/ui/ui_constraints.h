#pragma once

#include <lev/core/util.h>

namespace lev::ui
{
	class UIComponent;

	class UIConstraint
	{
	public:
		UIConstraint() = default;
		virtual ~UIConstraint() = default;

		virtual int constrain_x(UIComponent& component) = 0;
		virtual int constrain_y(UIComponent& component) = 0;
		virtual int constrain_width(UIComponent& component) = 0;
		virtual int constrain_height(UIComponent& component) = 0;
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

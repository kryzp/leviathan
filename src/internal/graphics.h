#pragma once

#include <lev/core/app.h>

namespace Lev
{
	enum class RendererType
	{
		None = -1,
		OpenGL
	};

	namespace Graphics
	{
		bool init();
		void shutdown();
		
		void before_render();
		void after_render();

		RendererType renderer_type();
	}
}

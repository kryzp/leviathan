#pragma once

#include <lev/core/app.h>

namespace LEV
{
	namespace Graphics
	{
		bool init();
		void shutdown();
		
		void before_render();
		void after_render();

		RendererType renderer_type();
	}
}

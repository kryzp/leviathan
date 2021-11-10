#pragma once

#include <lev/core/app.h>
#include <lev/core/util.h>

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

		u32 create_shader(const char* vertex, const char* fragment);
		void use_shader(u32 shader);
		void shader_set_u1i(u32 id, const char* name, int value);
		void shader_set_u1f(u32 id, const char* name, float value);

		RendererType renderer_type();
	}
}
